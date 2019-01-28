/**
 * @file irq_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM IRQ Socket
 *
 * IRQ Socket
 */

#ifndef HV_CORE_TLM_2_PROTOCOLS_IRQ_SOCKET_H_
#define HV_CORE_TLM_2_PROTOCOLS_IRQ_SOCKET_H_

#include <tlm>
#include <HVCommon>
#include <HVConfiguration>
#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/sockets/target_socket.h"
#include "protocols/irq/irq_payload.h"
#include "protocols/irq/irq_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace irq {

template<typename TYPES = IRQProtocolTypes, int N = 1,
		sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class IRQInitiatorSocket: public InitiatorSocket<32,
		TYPES, N, POL> {
public:
	typedef InitiatorSocket<32, TYPES, N, POL> base_type;

	IRQInitiatorSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("IRQInitiatorSocket")) :
			base_type(name_) {

	}
};

template<typename TYPES = IRQProtocolTypes, int N = 1,
		sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class IRQTargetSocket: public TargetSocket<32,
		TYPES, N, POL> {
public:
	typedef TargetSocket<32, TYPES, N, POL> base_type;

	IRQTargetSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("IRQTargetSocket")) :
			base_type(name_) {

	}
};


}
}
}
}
}
#endif /* HV_CORE_TLM_2_PROTOCOLS_IRQ_SOCKET_H_ */
