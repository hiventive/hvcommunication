/**
 * @file i2c_payload.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C Payload
 *
 * I2C payload
 */

#ifndef HV_PROTOCOLS_I2C_I2C_PAYLOAD_H_
#define HV_PROTOCOLS_I2C_I2C_PAYLOAD_H_

#include "core/tlm-2/base_payload/base_payload.h"
#include "location/addressing_payload_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace i2c {

enum I2CCommand {
	I2C_READ_COMMAND, I2C_WRITE_COMMAND
};

enum I2CResponseStatus {
	I2C_ERROR_RESPONSE = 0, I2C_OK_RESPONSE = 1
};

/**
 * Type definition for default address type for I2C
 */
typedef ::hv::common::hvuint16_t i2c_address_type;

template<typename ADDR_TYPE> class I2CPayload: public BasePayload,
		public AddressingPayloadIf<ADDR_TYPE> {
public:
	typedef ADDR_TYPE address_type;

	I2CPayload();
	explicit I2CPayload(MemoryManagementIf* MM_);

	virtual ~I2CPayload();

	// Accessors
	I2CCommand getCommand() const;
	::hv::common::hvuint8_t* getDataPtr() const;
	::std::size_t getDataLength() const;
	I2CResponseStatus getResponseStatus() const;
	address_type getAddress() const;

	// Modifiers
	void setCommand(const I2CCommand& cmd);
	void setDataPtr(::hv::common::hvuint8_t* dataPtr);
	void setDataLength(const ::std::size_t& dataLen);
	void setResponseStatus(const I2CResponseStatus& responseStatus);
	void setAddress(const address_type& destAddr);

	// Helpers
	bool isResponseOk() const;
	bool isResponseError() const;
	::std::string getResponseString() const;

	// Deep copy
	bool deepCopyFrom(const I2CPayload& src);
	void updateExtensionsFrom(const I2CPayload& src);

private:
	I2CPayload(const I2CPayload&) /* = delete */;
	I2CPayload& operator =(const I2CPayload& src) /* = delete */;

protected:
	I2CCommand mCommand;
	::hv::common::hvuint8_t* mData;
	::std::size_t mLength;
	I2CResponseStatus mResponseStatus;
	address_type mDestinationAddress;
};

}
}
}
}
}

#include "i2c_payload.hpp"

#endif /* HV_PROTOCOLS_I2C_I2C_PAYLOAD_H_ */
