/**
 * @file signal_protocol.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Protocol
 *
 * Signal Protocol
 */

#ifndef HV_PROTOCOLS_SIGNAL_SIGNAL_PROTOCOL_H_
#define HV_PROTOCOLS_SIGNAL_SIGNAL_PROTOCOL_H_

#include "signal_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace signal {

struct SignalProtocolTypes {
	typedef SignalPayload tlm_payload_type;
	typedef ::tlm::tlm_phase tlm_phase_type;
};

}
}
}
}
}



#endif /* HV_PROTOCOLS_SIGNAL_SIGNAL_PROTOCOL	_H_ */
