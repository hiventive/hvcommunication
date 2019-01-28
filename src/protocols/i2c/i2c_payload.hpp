/**
 * @file i2c_payload.hpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C Payload
 *
 * I2C payload
 */

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace i2c {

template<typename ADDR_TYPE> I2CPayload<ADDR_TYPE>::I2CPayload() :
		BasePayload(), mCommand(I2C_READ_COMMAND), mData(nullptr), mLength(0u), mResponseStatus(
				I2C_OK_RESPONSE), mDestinationAddress(0u) {

}

template<typename ADDR_TYPE> I2CPayload<ADDR_TYPE>::I2CPayload(
		MemoryManagementIf* MM_) :
		BasePayload(MM_), mCommand(I2C_READ_COMMAND), mData(nullptr), mLength(
				0u), mResponseStatus(I2C_OK_RESPONSE), mDestinationAddress(0u) {

}

template<typename ADDR_TYPE> I2CPayload<ADDR_TYPE>::~I2CPayload() {
}

// Accessors
template<typename ADDR_TYPE> I2CCommand I2CPayload<ADDR_TYPE>::getCommand() const {
	return mCommand;
}

template<typename ADDR_TYPE> ::hv::common::hvuint8_t* I2CPayload<ADDR_TYPE>::getDataPtr() const {
	return mData;
}

template<typename ADDR_TYPE> ::std::size_t I2CPayload<ADDR_TYPE>::getDataLength() const {
	return mLength;
}

template<typename ADDR_TYPE> I2CResponseStatus I2CPayload<ADDR_TYPE>::getResponseStatus() const {
	return mResponseStatus;
}

template<typename ADDR_TYPE> ADDR_TYPE I2CPayload<ADDR_TYPE>::getAddress() const {
	return mDestinationAddress;
}

// Modifiers
template<typename ADDR_TYPE> void I2CPayload<ADDR_TYPE>::setCommand(
		const I2CCommand& cmd) {
	mCommand = cmd;
}

template<typename ADDR_TYPE> void I2CPayload<ADDR_TYPE>::setDataPtr(
		::hv::common::hvuint8_t* dataPtr) {
	mData = dataPtr;
}

template<typename ADDR_TYPE> void I2CPayload<ADDR_TYPE>::setDataLength(
		const ::std::size_t& dataLen) {
	mLength = dataLen;
}

template<typename ADDR_TYPE> void I2CPayload<ADDR_TYPE>::setResponseStatus(
		const I2CResponseStatus& responseStatus) {
	mResponseStatus = responseStatus;
}

template<typename ADDR_TYPE> void I2CPayload<ADDR_TYPE>::setAddress(
		const ADDR_TYPE& addr_) {
	mDestinationAddress = addr_;
}

// Helpers
template<typename ADDR_TYPE> bool I2CPayload<ADDR_TYPE>::isResponseOk() const {
	return this->mResponseStatus > 0;
}
template<typename ADDR_TYPE> bool I2CPayload<ADDR_TYPE>::isResponseError() const {
	return !this->mResponseStatus;
}

template<typename ADDR_TYPE> ::std::string I2CPayload<ADDR_TYPE>::getResponseString() const {
	std::string ret;
	switch (mResponseStatus) {
	case I2C_OK_RESPONSE:
		ret = "I2C_OK_RESPONSE";
		break;
	default:
		ret = "I2C_ERROR_RESPONSED";
		break;
	}
	return ret;
}

// Deep copy
template<typename ADDR_TYPE> bool I2CPayload<ADDR_TYPE>::deepCopyFrom(
		const I2CPayload& src) {
	mCommand = src.getCommand();
	mData = src.getDataPtr();
	mLength = src.getDataLength();
	mResponseStatus = src.getResponseStatus();
	mDestinationAddress = src.getAddress();
	// Copying extensions
	return BasePayload::deepCopyFrom(src);
}

template<typename ADDR_TYPE> void I2CPayload<ADDR_TYPE>::updateExtensionsFrom(
		const I2CPayload& src) {
	BasePayload::updateExtensionsFrom((BasePayload&) src);
}

}
}
}
}
}

