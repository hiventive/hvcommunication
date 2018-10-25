/**
 * @file addressable_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Addressable Socket
 *
 * A socket should inherit this class if it is addressable.
 * This insures a full compatibility with Hiventive's routers
 */

#ifndef HV_CORE_TLM_2_SOCKETS_ADDRESSABLE_SOCKET_H_
#define HV_CORE_TLM_2_SOCKETS_ADDRESSABLE_SOCKET_H_

#include <hv/common.h>
#include <hv/configuration.h>

#include "../../../location/location.h"

namespace hv {
namespace communication {
namespace tlm2 {

template<typename ADDRESS_TYPE = ::hv::common::hvaddr_t,
		typename SIZE_TYPE = std::size_t> class AddressableSocket {
public:
	typedef ADDRESS_TYPE address_type;
	typedef SIZE_TYPE size_type;
	typedef Location<address_type, size_type> location_type;

	explicit AddressableSocket(const std::string& baseName_,
			const location_type &loc_ = location_type(
					static_cast<address_type>(0u))) :
			location(baseName_ + HV_SYSTEMC_HIERARCHY_CHAR + "location", loc_, "", ::cci::CCI_ABSOLUTE_NAME) {
	}

	explicit AddressableSocket(const std::string& baseName_,
			const address_type& addr_, const size_type &size_ = 0u) :
			location(baseName_ + HV_SYSTEMC_HIERARCHY_CHAR + "location", location_type(addr_, size_), "",
					::cci::CCI_ABSOLUTE_NAME) {

	}

	location_type getLocation() const {
		return location;
	}

protected:
	::hv::cfg::Param<location_type> location;
};

}
}
}

#endif /* HV_CORE_TLM_2_SOCKETS_ADDRESSABLE_SOCKET_H_ */

