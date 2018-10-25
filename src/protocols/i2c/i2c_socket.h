/**
 * @file i2c_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C Socket
 *
 * I2C Socket
 */

#ifndef HV_PROTOCOLS_I2C_I2C_SOCKET_H_
#define HV_PROTOCOLS_I2C_I2C_SOCKET_H_

#include <tlm>
#include <hv/common.h>
#include <hv/configuration.h>

#include "../../core/tlm-2/sockets/initiator_socket.h"
#include "../../core/tlm-2/sockets/addressable_socket.h"
#include "../../core/tlm-2/sockets/target_socket.h"
#include "../../protocols/i2c/i2c_payload.h"
#include "../../protocols/i2c/i2c_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace i2c {

template<typename TYPES = I2CProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class I2CInitiatorSocket: public InitiatorSocket<
		32, TYPES, N, POL>,
		public AddressableSocket<i2c_address_type> {
public:
	typedef InitiatorSocket<32, TYPES, N, POL> base_type;
	typedef AddressableSocket<i2c_address_type> addr_base_type;

	I2CInitiatorSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("I2CInitiatorSocket"),
			const i2c_address_type& addr_ = 0u) :
			base_type(name_), addr_base_type(this->name(), addr_) {
				// Adding base IO
				IOElement SDA("SDA", INOUT_MODE);
				IOElement SCL("SCL", INOUT_MODE);
				BaseIO baseIOTmp;
				baseIOTmp.addIOElement(SDA);
				baseIOTmp.addIOElement(SCL);
				this->addBaseIO(baseIOTmp);
				this->addBindingRule("SDA", "SDA");
				this->addBindingRule("SCL", "SCL");
	}

	virtual ~I2CInitiatorSocket() {
	}
};

template<typename TYPES = I2CProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class I2CTargetSocket: public TargetSocket<32,
		TYPES, N, POL>,
		public AddressableSocket<i2c_address_type> {
public:
	typedef TargetSocket<32, TYPES, N, POL> base_type;
	typedef AddressableSocket<i2c_address_type> addr_base_type;

	I2CTargetSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("I2CInitiatorSocket"),
			const i2c_address_type& addr_ = 0u) :
			base_type(name_), addr_base_type(this->name(), addr_) {
				// Adding base IO
				BaseIO baseIOTmp;
				baseIOTmp.addIOElement("SDA", INOUT_MODE);
				baseIOTmp.addIOElement("SCL", INOUT_MODE);
				this->addBaseIO(baseIOTmp);
				this->addBindingRule("SDA", "SDA");
				this->addBindingRule("SCL", "SCL");
	}

	virtual ~I2CTargetSocket() {
	}
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_I2C_I2C_SOCKET_H_ */
