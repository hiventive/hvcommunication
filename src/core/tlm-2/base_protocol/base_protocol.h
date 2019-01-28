/**
 * @file base_protocol.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM base protocol
 *
 * This is genuinely just a typedef to have the right style and right namespace
 */

#ifndef HV_CORE_TLM_2_PROTOCOL_BASE_PROTOCOL_H_
#define HV_CORE_TLM_2_PROTOCOL_BASE_PROTOCOL_H_

#include <tlm>

namespace hv {
namespace communication {
namespace tlm2 {

typedef ::tlm::tlm_base_protocol_types BaseProtocolTypes;

}
}
}


#endif /* HV_CORE_TLM_2_PROTOCOL_BASE_PROTOCOL_H_ */
