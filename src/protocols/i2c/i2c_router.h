/**
 * @file i2c_router.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C Router
 *
 * I2C Router
 */

#ifndef HV_PROTOCOLS_I2C_I2C_ROUTER_H_
#define HV_PROTOCOLS_I2C_I2C_ROUTER_H_

#include "../../protocols/i2c/i2c_protocol.h"
#include "../../utils/tlm-2/routing/generic_router.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace i2c {

class I2CRouter: public ::hv::communication::tlm2::utils::GenericRouter<32,
		I2CProtocolTypes, Location<i2c_address_type>> {
public:
	typedef ::hv::communication::tlm2::utils::GenericRouter<32,
			I2CProtocolTypes, Location<i2c_address_type>> base_type;

	I2CRouter(
			::sc_core::sc_module_name name_ = ::sc_core::sc_gen_unique_name(
					"I2CRouter")) :
			base_type(name_, false) {
	}
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_I2C_I2C_ROUTER_H_ */
