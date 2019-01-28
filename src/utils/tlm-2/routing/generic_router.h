/**
 * @file generic_router.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Generic Router
 */

#ifndef HV_ROUTING_GENERIC_ROUTER_H_
#define HV_ROUTING_GENERIC_ROUTER_H_

#include <algorithm>
#include <map>
#include <HVCommon>
#include <HVConfiguration>
#include <systemc>
#include "utils/tlm-2/sockets/multi_initiator_socket.h"
#include "utils/tlm-2/sockets/multi_target_socket.h"
#include "location/location.h"
#include "payload_address_handler.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

template<unsigned int BUSWIDTH = 32,
		typename TYPES = ::hv::communication::tlm2::BaseProtocolTypes,
		typename LOCATION_TYPE = ::hv::communication::tlm2::Location<>> class GenericRouter: public ::sc_core::sc_module {
public:
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;

	typedef LOCATION_TYPE location_type;
	typedef typename location_type::address_type address_type;

	typedef PayloadAddressHandler<TYPES, LOCATION_TYPE> address_handler_type;

	typedef tlm::tlm_sync_enum sync_enum_type;

	GenericRouter(
			::sc_core::sc_module_name name_ = sc_core::sc_gen_unique_name(
					"GenericRouter"), const bool& setRelativeAddress_ = false) :
			::sc_core::sc_module(name_), inputSocket("InputSocket"), outputSocket(
					"OutputSocket"), mBrokerHandle(::cci::cci_get_broker()), mSetRelativeAddress(
					setRelativeAddress_) {
		// Registering callbacks
		inputSocket.registerBTransport(this, &GenericRouter::routerBTransport);
		inputSocket.registerNBTransportFw(this,
				&GenericRouter::routerNBTransportFw);
		inputSocket.registerTransportDbg(this,
				&GenericRouter::routerTransportDbg);
		inputSocket.registerGetDirectMemPtr(this,
				&GenericRouter::routerGetDirectMemPtr);
		outputSocket.registerNBTransportBw(this,
				&GenericRouter::routerNBTransportBw);
		outputSocket.registerInvalidateDirectMemPtr(this,
				&GenericRouter::routerInvalidateDirectMemPtr);
	}

	::hv::communication::tlm2::utils::MultiTargetSocket<BUSWIDTH, TYPES> inputSocket;
	::hv::communication::tlm2::utils::MultiInitiatorSocket<BUSWIDTH, TYPES> outputSocket;

	void end_of_elaboration() {
		// We don't know who we were bound to. Now we just manage everything
		// Getting output sockets (bound to our MultiInitiatorSocket)
		std::vector<
				typename ::hv::communication::tlm2::utils::MultiInitiatorSocket<
						BUSWIDTH, TYPES>::base_target_socket_type*>& outputInterfaces =
				outputSocket.getSockets();
		for (auto it = outputInterfaces.begin(); it != outputInterfaces.end();
				++it) {
			// Getting location (fails if there isn't)
			location_type loc(getLocation(*it));
			// Checking if this location is free in our router
			if (!isOutputLocationFree(loc)) {
				HV_ERR("Location not free")
				exit(EXIT_FAILURE);
			}
			// Otherwise, adding interface to our map
			mOutputSocketMap.insert(
					std::pair<location_type,
							typename ::hv::communication::tlm2::FwBwTransportIf<
									TYPES>::fw_interface_type*>(loc,
							&(*it)->get_base_interface()));
		}

	}

protected:
	typedef typename std::map<location_type,
			typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type*>::const_iterator location_map_cit_type;

	//** Callbacks (FW interface) **//
	void routerBTransport(const std::size_t& id, transaction_type& trans,
			sc_core::sc_time& t) {
		// Getting address from payload
		address_type destinationAddr(mAddressHandler.getAddress(trans));
		// Getting corresponding interface
		location_map_cit_type it;
		if (getOutputSocketMapIterator(destinationAddr, &it)) {
			// If found, set relative address in payload if necessary,
			// subtracting an offset equal to starting address of location
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						destinationAddr - it->first.getAddress());
			}
			// Transmitting callback to the right interface type
			(*(it->second)).b_transport(trans, t);
			// Setting re-applying inverse address offset
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						mAddressHandler.getAddress(trans)
								+ it->first.getAddress());
			}
		}
	}

	sync_enum_type routerNBTransportFw(const std::size_t& id,
			transaction_type& trans, phase_type& phase, sc_core::sc_time& t) {
		// Getting address from payload
		address_type destinationAddr(mAddressHandler.getAddress(trans));
		// Getting corresponding interface
		location_map_cit_type it;
		if (getOutputSocketMapIterator(destinationAddr, &it)) {
			// If found, set relative address in payload if necessary,
			// subtracting an offset equal to starting address of location
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						destinationAddr - it->first.getAddress());
			}
			// TODO find target ID and remember it
			// Getting output socket list
			auto socketsTmp = outputSocket.getSockets();
			// Looking for the socket having the same interface address as
			// the one we are communicating with
			bool sNotFound(true);
			for (std::size_t i = 0u; sNotFound && (i < socketsTmp.size());
					++i) {
				if (&socketsTmp[i]->get_base_interface() == it->second) {
					// When found, adding to our list
					mLatestNBCommunication[std::size_t(i)] = id;
					sNotFound = false;
				}
			}
			if (sNotFound) {
				HV_ERR("Socket was not found in target list")
				exit(EXIT_FAILURE);
			}
			// Transmitting callback to the right interface
			sync_enum_type ret = (*(it->second)).nb_transport_fw(trans, phase,
					t);
			// Setting re-applying inverse address offset
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						mAddressHandler.getAddress(trans)
								+ it->first.getAddress());
			}
			return ret;
		}
		return sync_enum_type::TLM_COMPLETED;
	}

	unsigned int routerTransportDbg(const std::size_t& id,
			transaction_type& trans) {
		// Getting address from payload
		address_type destinationAddr(mAddressHandler.getAddress(trans));
		// Getting corresponding interface
		location_map_cit_type it;
		if (getOutputSocketMapIterator(destinationAddr, &it)) {
			// If found, set relative address in payload if necessary,
			// subtracting an offset equal to starting address of location
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						destinationAddr - it->first.getAddress());
			}
			// Transmitting callback to the right interface
			unsigned int ret = (*(it->second)).transport_dbg(trans);
			// Setting re-applying inverse address offset
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						mAddressHandler.getAddress(trans)
								+ it->first.getAddress());
			}
			return ret;
		}
		return 0u;
	}

	bool routerGetDirectMemPtr(const std::size_t& id, transaction_type& trans,
			tlm::tlm_dmi& dmi_data) {
		// Getting address from payload
		address_type destinationAddr(mAddressHandler.getAddress(trans));
		// Getting corresponding interface
		location_map_cit_type it;
		if (getOutputSocketMapIterator(destinationAddr, &it)) {
			// If found, set relative address in payload if necessary,
			// subtracting an offset equal to starting address of location
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						destinationAddr - it->first.getAddress());
			}
			// Transmitting callback to the right interface
			bool ret = (*(it->second)).get_direct_mem_ptr(trans, dmi_data);
			// Restoring offset
			if (mSetRelativeAddress) {
				mAddressHandler.setAddress(trans,
						mAddressHandler.getAddress(trans)
								+ it->first.getAddress());
				// Applying offset on dmi_data
				if (!dmi_data.is_none_allowed()) {
					dmi_data.set_start_address(
							dmi_data.get_start_address()
									+ it->first.getAddress());
					dmi_data.set_end_address(
							dmi_data.get_end_address()
									+ it->first.getAddress());
				}
			}
			return ret;
		}
		return false;
	}

	//** Callbacks (BW interface) **//
	sync_enum_type routerNBTransportBw(const std::size_t& id,
			transaction_type& trans, phase_type& phase, sc_core::sc_time& t) {
		// Getting ID of the initiator socket that initiated the exchange
		auto it = mLatestNBCommunication.find(id);
		if (it == mLatestNBCommunication.end()) {
			HV_ERR(
					"No nb_transport_fw was initiated by initiator before call to nb_transport_bw by the target. I don't know where to transmit it")
			exit(EXIT_FAILURE);
		}
		return inputSocket[it->second]->nb_transport_bw(trans, phase, t);
	}

	void routerInvalidateDirectMemPtr(const std::size_t& id,
			sc_dt::uint64 startRange, sc_dt::uint64 endRange) {
		auto inputSockets = inputSocket.getSockets();
		for (auto &it : inputSockets) {
			it->invalidate_direct_mem_ptr(startRange, endRange);
		}
	}

	//** Helpers **//
	inline bool getOutputSocketMapIterator(const address_type& addr,
			location_map_cit_type* it) {
		// Searching for address
		location_type locTmp(addr);
		*it = mOutputSocketMap.find(locTmp);
		bool ret(*it != mOutputSocketMap.cend());
		if (!ret) {
			HV_WARN(
					"Address 0x" << std::hex << addr << std::dec << " has no destination in router")
		}
		return ret;
	}

	location_type getLocation(
			typename ::hv::communication::tlm2::utils::MultiInitiatorSocket<
					BUSWIDTH, TYPES>::base_target_socket_type* s) {

		std::string baseName(std::string(s->get_base_export().name()));
		::cci::cci_param_untyped_handle h(
				mBrokerHandle.get_param_handle(
						baseName + HV_SYSTEMC_HIERARCHY_CHAR + "location"));
		if (!(h.is_valid())) {
			// This could be a bidirectional socket
			h = ::cci::cci_param_untyped_handle(
					mBrokerHandle.get_param_handle(
							baseName.substr(0,
									baseName.size()
											- std::string(".Target").size())
									+ HV_SYSTEMC_HIERARCHY_CHAR + "location"));
			if (!(h.is_valid())) {
				HV_ERR(
						"Impossible to bind a router with a socket which has no location")
				exit(EXIT_FAILURE);
			}
		}
		::cci::cci_param_typed_handle<location_type> loc(h);
		return loc.get_value();
	}

public:
	bool isOutputLocationFree(const location_type& loc) const {
		return (mOutputSocketMap.find(loc) == mOutputSocketMap.end());
	}

protected:
	std::map<location_type,
			typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type*> mOutputSocketMap;
	std::map<std::size_t, std::size_t> mLatestNBCommunication;
	::cci::cci_broker_handle mBrokerHandle;
	bool mSetRelativeAddress;
	address_handler_type mAddressHandler;
};

} // closing namespaces
}
}
}

#endif /* HV_ROUTING_GENERIC_ROUTER_H_ */
