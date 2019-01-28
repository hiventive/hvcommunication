/**
 * @file i2c_simple_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C Simple Socket
 *
 * I2C Simple Socket
 */

#ifndef HV_PROTOCOLS_I2C_I2C_SIMPLE_SOCKET_H_
#define HV_PROTOCOLS_I2C_I2C_SIMPLE_SOCKET_H_

#include "protocols/i2c/i2c_protocol.h"
#include "utils/tlm-2/sockets/simple_initiator_socket.h"
#include "utils/tlm-2/sockets/simple_target_socket.h"
#include "core/tlm-2/sockets/addressable_socket.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace i2c {

template<typename TYPES = I2CProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ZERO_OR_MORE_BOUND> class I2CSimpleInitiatorSocket: public ::hv::communication::tlm2::utils::SimpleInitiatorSocket<
		32, TYPES, N, POL>,
		public AddressableSocket<i2c_address_type> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleInitiatorSocket<32, TYPES, N,
			POL> base_type;
	typedef AddressableSocket<i2c_address_type> addr_base_type;

	I2CSimpleInitiatorSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("I2CSimpleInitiatorSocket"),
			const i2c_address_type& addr_ = 0u) :
			base_type(name_), addr_base_type(this->name(), addr_) {
				// Adding base IO
				IOElement SDA("SDA", INOUT_MODE);
				IOElement SCL("SCL", INOUT_MODE);
				BaseIO baseIOTmp;
				baseIOTmp.addIOElement(SDA);
				baseIOTmp.addIOElement(SCL);
				this->addBaseIO(baseIOTmp);
	}

	virtual ~I2CSimpleInitiatorSocket() {
	}
};

template<typename TYPES = I2CProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ZERO_OR_MORE_BOUND> class I2CSimpleTargetSocket: public ::hv::communication::tlm2::utils::SimpleTargetSocket<
		32, TYPES, N, POL>,
		public AddressableSocket<i2c_address_type> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleTargetSocket<32, TYPES, N, POL> base_type;
	typedef AddressableSocket<i2c_address_type> addr_base_type;

	I2CSimpleTargetSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("I2CSimpleTargetSocket"),
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

	virtual ~I2CSimpleTargetSocket() {
	}
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_I2C_I2C_SIMPLE_SOCKET_H_ */
