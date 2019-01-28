/**
 * @file uart_protocol.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Protocol
 *
 * UART Protocol
 */

#ifndef HV_CORE_TLM_2_PROTOCOLS_IRQ_PROTOCOL_H_
#define HV_CORE_TLM_2_PROTOCOLS_IRQ_PROTOCOL_H_

#include"core/tlm-2/base_protocol/base_protocol.h"
#include"protocols/irq/irq_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace irq {

struct IRQProtocolTypes {
	typedef IRQPayload tlm_payload_type;
	typedef ::tlm::tlm_phase tlm_phase_type;
};

}
}
}
}
}

#endif /* HV_CORE_TLM_2_PROTOCOLS_IRQ_PROTOCOL_H_ */
