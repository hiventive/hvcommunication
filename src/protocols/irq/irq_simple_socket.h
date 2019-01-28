/**
 * @file irq_simple_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM IRQ Convenient Sockets
 *
 * IRQ Socket
 */

#ifndef HV_CORE_TLM_2_PROTOCOLS_IRQ_SIMPLE_SOCKET_H_
#define HV_CORE_TLM_2_PROTOCOLS_IRQ_SIMPLE_SOCKET_H_

#include <tlm>
#include <HVCommon>
#include <HVConfiguration>
#include "utils/tlm-2/sockets/simple_initiator_socket.h"
#include "utils/tlm-2/sockets/simple_target_socket.h"
#include "protocols/irq/irq_payload.h"
#include "protocols/irq/irq_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace irq {

template<typename TYPES = IRQProtocolTypes, int N = 1,
		sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class IRQSimpleInitiatorSocket: public ::hv::communication::tlm2::utils::SimpleInitiatorSocket<
		32, TYPES, N, POL> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleInitiatorSocket<32, TYPES, N, POL> base_type;

	IRQSimpleInitiatorSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("IRQSimpleInitiatorSocket")) :
			base_type(static_cast<::sc_core::sc_module_name>(name_)) {

	}
};

template<typename TYPES = IRQProtocolTypes, int N = 1,
		sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class IRQSimpleTargetSocket: public ::hv::communication::tlm2::utils::SimpleTargetSocket<
		32, TYPES, N, POL> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleTargetSocket<32, TYPES, N, POL> base_type;

	IRQSimpleTargetSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("IRQSimpleTargetSocket")) :
			base_type(static_cast<::sc_core::sc_module_name>(name_)) {

	}
};

}
}
}
}
}
#endif /* HV_CORE_TLM_2_PROTOCOLS_IRQ_SIMPLE_SOCKET_H_ */
