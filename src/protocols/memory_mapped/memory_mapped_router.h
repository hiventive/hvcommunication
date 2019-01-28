/**
 * @file memory_mapped_router.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory-Mapped Router
 *
 * Memory-Mapped Router
 */

#ifndef HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_ROUTER_H_
#define HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_ROUTER_H_

#include "protocols/memory_mapped/memory_mapped_protocol.h"
#include "utils/tlm-2/routing/generic_router.h"
#include <HVConfiguration>

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace memorymapped {

/**
 * The memory-mapped router is a generic router with absolute-to-relative
 * address translation.
 * Therefore it inherits GenericRouter and forces translation
 */
template<unsigned int BUSWIDTH = 32> class MemoryMappedRouter: public ::hv::communication::tlm2::utils::GenericRouter<
		BUSWIDTH, MemoryMappedProtocolTypes,
		Location<::hv::common::hvaddr_t, ::std::size_t>> {

public:
	typedef ::hv::communication::tlm2::utils::GenericRouter<BUSWIDTH,
			MemoryMappedProtocolTypes,
			Location<::hv::common::hvaddr_t, ::std::size_t>> base_type;

	MemoryMappedRouter(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("MemoryMappedRouter")) :
			base_type(name_, true) {
	}

};

}
}
}
}
}

#endif /* HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_ROUTER_H_ */
