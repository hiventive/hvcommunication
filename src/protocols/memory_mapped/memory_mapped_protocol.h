/**
 * @file memory_mapped_protocol.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory-Mapped Protocol
 *
 * Memory-Mapped Protocol
 */

#ifndef HV_CORE_TLM_2_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PROTOCOL_H_
#define HV_CORE_TLM_2_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PROTOCOL_H_

#include <hv/common.h>
#include "../../core/tlm-2/base_protocol/base_protocol.h"
#include "../../protocols/memory_mapped/memory_mapped_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace memorymapped {

struct MemoryMappedProtocolTypes {
	typedef MemoryMappedPayload<::hv::common::hvaddr_t> tlm_payload_type;
	typedef ::tlm::tlm_phase tlm_phase_type;
};

}
}
}
}
}

#endif /* HV_CORE_TLM_2_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PROTOCOL_H_ */
