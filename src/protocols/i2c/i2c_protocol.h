/**
 * @file i2c_protocol.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C Protocol
 *
 * I2C Protocol
 */

#ifndef HV_CORE_TLM_2_PROTOCOLS_I2C_I2C_PROTOCOL_H_
#define HV_CORE_TLM_2_PROTOCOLS_I2C_I2C_PROTOCOL_H_

#include <HVCommon>
#include"core/tlm-2/base_protocol/base_protocol.h"
#include"protocols/i2c/i2c_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace i2c {

struct I2CProtocolTypes {
	typedef I2CPayload<::hv::common::hvuint16_t> tlm_payload_type;
	typedef ::tlm::tlm_phase tlm_phase_type;
};

}
}
}
}
}

#endif /* HV_CORE_TLM_2_PROTOCOLS_I2C_I2C_PROTOCOL_H_ */
