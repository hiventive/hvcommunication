/**
 * @file target_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM base target socket
 *
 * This is just made to get rid of the bus size as we just want a general purpose socket
 */

#ifndef HV_CORE_TLM_2_SOCKETS_TARGET_SOCKET_H_
#define HV_CORE_TLM_2_SOCKETS_TARGET_SOCKET_H_

#include "core/tlm-2/base_protocol/base_protocol.h"
#include "core/tlm-2/interfaces/fw_bw_transport_if.h"
#include <tlm>

#include "base_socket.h"
namespace hv {
namespace communication {
namespace tlm2 {

template<unsigned int BUSWIDTH, typename FW_IF, typename BW_IF, int N, sc_core::sc_port_policy POL> class BaseInitiatorSocket;

template<unsigned int BUSWIDTH = 32, typename FW_IF = FwBwTransportIf<>::fw_interface_type,
typename BW_IF = FwBwTransportIf<>::bw_interface_type, int N = 1,
sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND>
class BaseTargetSocket: public ::tlm::tlm_base_target_socket<BUSWIDTH, FW_IF, BW_IF,
N, POL>, public BaseSocket {

public:
	typedef ::tlm::tlm_base_target_socket<BUSWIDTH, FW_IF, BW_IF, N, POL> base_t;
	template<unsigned int, typename, typename, int, sc_core::sc_port_policy> friend class BaseInitiatorSocket;

	BaseTargetSocket() :
	base_t(), BaseSocket() {
	}

	explicit BaseTargetSocket(const char *str) :
	base_t(str) {
	}
};

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N = 1,
sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class TargetSocket: public BaseTargetSocket<BUSWIDTH, typename
FwBwTransportIf<TYPES>::fw_interface_type, typename FwBwTransportIf<TYPES>::bw_interface_type, N,
POL> {
	typedef BaseTargetSocket<BUSWIDTH, typename
	FwBwTransportIf<TYPES>::fw_interface_type, typename FwBwTransportIf<TYPES>::bw_interface_type, N,
	POL> base_t;
public:
	TargetSocket() :
	base_t() {
	}

	explicit TargetSocket(const char* name) :
	base_t(name) {
	}

	virtual ~TargetSocket() {

	}

	virtual const char* kind() const {
		return "TargetSocket";
	}

	virtual sc_core::sc_type_index get_protocol_types() const {
		return typeid(TYPES);
	}
};


}
}
}

#endif /* HV_CORE_TLM_2_SOCKETS_TARGET_SOCKET_H_ */
