/**
 * @file memory_mapped_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory-Mapped Socket
 *
 * Memory-Mapped Socket
 */

#ifndef HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_SOCKET_H_
#define HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_SOCKET_H_

#include <tlm>
#include <HVCommon>
#include <HVConfiguration>
#include "core/tlm-2/sockets/bidirectional_socket.h"
#include "core/tlm-2/sockets/addressable_socket.h"
#include "protocols/memory_mapped/memory_mapped_payload.h"
#include "protocols/memory_mapped/memory_mapped_protocol.h"
#include "protocols/memory_mapped/memory_mapped_protocol_converters.h"
#include "core/tlm-2/protocol_adapter/protocol_adapter.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace memorymapped {

template<unsigned int BUSWIDTH = 32, typename TYPES = MemoryMappedProtocolTypes, int N_INIT = 1,
		int N_TARG = 1, sc_core::sc_port_policy POL_INIT =
				sc_core::SC_ONE_OR_MORE_BOUND,
		sc_core::sc_port_policy POL_TARG = sc_core::SC_ONE_OR_MORE_BOUND> class MemoryMappedBidirectionalSocket: public BidirectionalSocket<
		BUSWIDTH, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG>,
		public AddressableSocket<::hv::common::hvaddr_t, ::std::size_t> {
public:
	typedef BidirectionalSocket<BUSWIDTH, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG> base_type;
	typedef AddressableSocket<::hv::common::hvaddr_t, ::std::size_t> addr_base_type;

	MemoryMappedBidirectionalSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("MemoryMappedBidirectionalSocket"),
			const ::hv::common::hvaddr_t& startAddr_ = 0u,
			const ::std::size_t size_ = 0u) :
			base_type(name_), addr_base_type(this->name(), startAddr_, size_) {
	}

	virtual ~MemoryMappedBidirectionalSocket() {

	}

};

template<unsigned int BUSWIDTH = 32, typename TYPES = MemoryMappedProtocolTypes, int N = 1,
		sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class MemoryMappedInitiatorSocket: public InitiatorSocket<
		BUSWIDTH, TYPES, N, POL> {
public:
	typedef InitiatorSocket<BUSWIDTH, TYPES, N, POL> base_type;
	typedef ::tlm::tlm_base_target_socket_b<BUSWIDTH,
			::tlm::tlm_fw_transport_if<::tlm::tlm_base_protocol_types>,
			::tlm::tlm_bw_transport_if<::tlm::tlm_base_protocol_types>> generic_target_type;
	typedef ::tlm::tlm_base_initiator_socket_b<BUSWIDTH,
			::tlm::tlm_fw_transport_if<::tlm::tlm_base_protocol_types>,
			::tlm::tlm_bw_transport_if<::tlm::tlm_base_protocol_types>> generic_initiator_type;
	using base_type::bind;

	MemoryMappedInitiatorSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("MemoryMappedInitiatorSocket")) :
			base_type(name_), mGenericProtocolAdapter("ProtocolAdapter",
					mConverter), hasBeenBound(false) {
	}

	virtual ~MemoryMappedInitiatorSocket() {

	}

	// Binding to ::tlm::tlm_base_target_socket_b
	void bind(generic_target_type& s) {
		mGenericProtocolAdapter.outputSocket.bind(s);
		hasBeenBound = true;
	}

	void operator()(generic_target_type& s) {
		bind(s);
	}

	// Binding to ::tlm::tlm_base_initiator_socket_b
	void bind(generic_initiator_type& s) {
		mGenericProtocolAdapter.outputSocket.bind(s);
		hasBeenBound = true;
	}

	void operator()(generic_initiator_type& s) {
		bind(s);
	}
protected:

	void before_end_of_elaboration() {
		// If the adapter has been bound outside, it must be bound inside
		if (hasBeenBound) {
			base_type::bind(mGenericProtocolAdapter.inputSocket);
		} else {
			// Clogging
			// Adapter is useless, we just clog the sockets to avoid SystemC errors
			mGenericProtocolAdapter.inputSocketClog.bind(
					mGenericProtocolAdapter.inputSocket);
			mGenericProtocolAdapter.outputSocket.bind(
					mGenericProtocolAdapter.outputSocketClog);
		}

	}

	::hv::communication::tlm2::protocols::memorymapped::TLMGenericMemoryMapProtocolConverter mConverter;
	::hv::communication::tlm2::ProtocolAdapter<TYPES, 1,
			::sc_core::SC_ONE_OR_MORE_BOUND, ::tlm::tlm_base_protocol_types, N,
			POL, false, BUSWIDTH> mGenericProtocolAdapter;
	bool hasBeenBound;
};

template<unsigned int BUSWIDTH = 32, typename TYPES = MemoryMappedProtocolTypes, int N = 0,
		sc_core::sc_port_policy POL = sc_core::SC_ZERO_OR_MORE_BOUND> class MemoryMappedTargetSocket: public TargetSocket<
		BUSWIDTH, TYPES, N, POL>,
		public AddressableSocket<::hv::common::hvaddr_t, ::std::size_t> {
public:
	typedef TargetSocket<BUSWIDTH, TYPES, N, POL> base_type;
	typedef AddressableSocket<::hv::common::hvaddr_t, ::std::size_t> addr_base_type;
	typedef ::tlm::tlm_base_target_socket_b<BUSWIDTH,
			::tlm::tlm_fw_transport_if<::tlm::tlm_base_protocol_types>,
			::tlm::tlm_bw_transport_if<::tlm::tlm_base_protocol_types>> generic_type;

	MemoryMappedTargetSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("MemoryMappedTargetSocket"),
			const ::hv::common::hvaddr_t& startAddr_ = 0u,
			const ::std::size_t size_ = 0u) :
			base_type(name_), addr_base_type(this->name(), startAddr_, size_), mGenericProtocolAdapter(
					"ProtocolAdapter", mConverter) {
	}

	virtual ~MemoryMappedTargetSocket() {

	}

	operator generic_type&() {
		return mGenericProtocolAdapter.inputSocket;
	}

protected:

	void before_end_of_elaboration() {
		::std::size_t nAdapterBindings =
				mGenericProtocolAdapter.howManyInputBindings();
		if (nAdapterBindings) {
			if (N) {
				if (this->size() + nAdapterBindings > N) {
					HV_ERR(
							"[MemoryMappedTargetSocket] Too many bindings on socket");
					exit(EXIT_FAILURE);
				}
			}
			mGenericProtocolAdapter.outputSocket.bind(*this);
		} else {
			// Adapter is useless, we just clog the sockets to avoid SystemC errors
			mGenericProtocolAdapter.inputSocketClog.bind(
					mGenericProtocolAdapter.inputSocket);
			mGenericProtocolAdapter.outputSocket.bind(
					mGenericProtocolAdapter.outputSocketClog);
		}

	}

	::hv::communication::tlm2::protocols::memorymapped::TLMGenericMemoryMapProtocolConverter mConverter;
	::hv::communication::tlm2::ProtocolAdapter<::tlm::tlm_base_protocol_types,
			N, POL, TYPES, 1, ::sc_core::SC_ONE_OR_MORE_BOUND, true, BUSWIDTH> mGenericProtocolAdapter;
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_SOCKET_H_ */
