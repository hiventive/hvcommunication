/**
 * @file simple_initiator_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Convenient Initiator Socket
 *
 * This convenient socket accelerates development by providing the possibility to
 * register callbacks to overload interfaces methods.
 */

#ifndef HV_UTILS_SOCKETS_SIMPLE_INITIATOR_SOCKET_H_
#define HV_UTILS_SOCKETS_SIMPLE_INITIATOR_SOCKET_H_

#include <tlm>
#include <HVCommon>
#include "utils/tlm-2/sockets/simple_socket_base.h"
#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/interfaces/fw_bw_transport_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N =
		1, sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class SimpleInitiatorSocketB: public InitiatorSocket<BUSWIDTH, TYPES, N, POL>,
		protected SimpleSocketBase {
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;
	typedef typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type fw_interface_type;
	typedef typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::bw_interface_type bw_interface_type;
	typedef InitiatorSocket<BUSWIDTH, TYPES, N, POL> base_type;

public:
	typedef ::hv::common::CallbackImpl<
			sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)> TransportCb;
	typedef ::hv::common::CallbackImpl<void(sc_dt::uint64, sc_dt::uint64)> InvalidateDirectMemPtrCb;

	SimpleInitiatorSocketB(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("SimpleInitiatorSocket")) :
			base_type(name_), mProcess() {
		this->bind(mProcess);
	}

	void registerNBTransportBw(const TransportCb &cb) {
		mProcess.setTransportPtr(cb);
	}

	// Helper function to avoid manual binding
	template<typename MODULE> void registerNBTransportBw(MODULE* mod,
			sync_enum_type (MODULE::*cb)(transaction_type&, phase_type&,
					sc_core::sc_time&)) {
		TransportCb cbTmp(cb, mod);
		this->registerNBTransportBw(cbTmp);
	}

	void registerInvalidateDirectMemPtr(const InvalidateDirectMemPtrCb &cb) {
		mProcess.setInvalidateDirectMemPtr(cb);
	}

	// Helper function to avoid manual binding
	template<typename MODULE> void registerInvalidateDirectMemPtr(MODULE *mod,
			void (MODULE::*cb)(sc_dt::uint64, sc_dt::uint64)) {
		InvalidateDirectMemPtrCb cbTmp(cb, mod);
		this->registerInvalidateDirectMemPtr(cbTmp);
	}

private:
	class process: public ::hv::communication::tlm2::FwBwTransportIf<TYPES>::bw_interface_type {
	public:

		process() :
				mTransportPtr(nullptr), mInvalidateDirectMemPtr(nullptr) {
		}

		void setTransportPtr(const TransportCb &p) {
			if (mTransportPtr) {
				HV_WARN(
						"Non-blocking callback already registered. Ignoring new registration")
				return;
			}
			mTransportPtr = p;
		}

		void setInvalidateDirectMemPtr(const InvalidateDirectMemPtrCb &p) {
			if (mInvalidateDirectMemPtr) {
				HV_WARN(
						"Invalidate DMI callback already registered. Ignoring new registration");
				return;
			}
			mInvalidateDirectMemPtr = p;
		}

		//** Interface implementation **//
		sync_enum_type nb_transport_bw(transaction_type& trans,
				phase_type& phase, sc_core::sc_time& t) {
			if (mTransportPtr) {
				return mTransportPtr(trans, phase, t);
			}
			HV_ERR("No transport callback registered");
			return tlm::TLM_COMPLETED;
		}
		void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
				sc_dt::uint64 end_range) {
			if (mInvalidateDirectMemPtr) {
				// forward call
				mInvalidateDirectMemPtr(start_range, end_range);
			}
		}
	private:
		TransportCb mTransportPtr;
		InvalidateDirectMemPtrCb mInvalidateDirectMemPtr;

	};
	process mProcess;
};

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N =
		1, ::sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class SimpleInitiatorSocket: public SimpleInitiatorSocketB<
		BUSWIDTH, TYPES, N, POL> {
	typedef SimpleInitiatorSocketB<BUSWIDTH, TYPES, N, POL> socket_b;
public:
	SimpleInitiatorSocket(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("SimpleInitiatorSocket")) :
			socket_b(name_) {
	}
};

}
}
}
}

#endif /* HV_UTILS_SOCKETS_SIMPLE_INITIATOR_SOCKET_H_ */
