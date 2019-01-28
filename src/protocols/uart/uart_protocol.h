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

#ifndef HV_CORE_TLM_2_PROTOCOLS_UART_UART_PROTOCOL_H_
#define HV_CORE_TLM_2_PROTOCOLS_UART_UART_PROTOCOL_H_

#include"core/tlm-2/base_protocol/base_protocol.h"
#include"protocols/uart/uart_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace uart {

struct UartProtocolTypes {
	typedef UartPayload tlm_payload_type;
	typedef ::tlm::tlm_phase tlm_phase_type;
};

}
}
}
}
}

#endif /* SRC_CORE_TLM_2_PROTOCOLS_UART_UART_PROTOCOL_H_ */
