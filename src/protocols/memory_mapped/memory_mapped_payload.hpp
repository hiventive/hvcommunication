/**
 * @file memory_mapped_payload.hpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory-Mapped Payload
 *
 * Memory-mapped payload implementation
 */

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace memorymapped {

template<typename ADDR_TYPE> MemoryMappedPayload<ADDR_TYPE>::MemoryMappedPayload() :
		BasePayload(), mCommand(MEM_MAP_READ_COMMAND), mData(nullptr), mLength(
				0u), mResponseStatus(MEM_MAP_OK_RESPONSE), mDestinationAddress(
				0u), mDMIAllowed(false) {

}

template<typename ADDR_TYPE> MemoryMappedPayload<ADDR_TYPE>::MemoryMappedPayload(
		MemoryManagementIf* MM_) :
		BasePayload(MM_), mCommand(MEM_MAP_READ_COMMAND), mData(nullptr), mLength(
				0u), mResponseStatus(MEM_MAP_OK_RESPONSE), mDestinationAddress(
				0u), mDMIAllowed(false) {

}

template<typename ADDR_TYPE> MemoryMappedPayload<ADDR_TYPE>::~MemoryMappedPayload() {
}

// Accessors
template<typename ADDR_TYPE> MemoryMappedCommand MemoryMappedPayload<ADDR_TYPE>::getCommand() const {
	return mCommand;
}

template<typename ADDR_TYPE> std::string MemoryMappedPayload<ADDR_TYPE>::getCommandStr() const {
	std::string ret("");
	switch (mCommand) {
	case MEM_MAP_READ_COMMAND:
		ret = "MEM_MAP_READ_COMMAND";
		break;
	case MEM_MAP_WRITE_COMMAND:
		ret = "MEM_MAP_WRITE_COMMAND";
		break;
	case MEM_MAP_IGNORE_COMMAND:
		ret = "MEM_MAP_IGNORE_COMMAND";
		break;
	default:
		HV_ERR("Unknown command")
		exit (EXIT_FAILURE);
	}
	return ret;
}

template<typename ADDR_TYPE> ::hv::common::hvuint8_t* MemoryMappedPayload<
		ADDR_TYPE>::getDataPtr() const {
	return mData;
}

template<typename ADDR_TYPE> ::std::size_t MemoryMappedPayload<ADDR_TYPE>::getDataLength() const {
	return mLength;
}

template<typename ADDR_TYPE> MemoryMappedResponseStatus MemoryMappedPayload<
		ADDR_TYPE>::getResponseStatus() const {
	return mResponseStatus;
}

template<typename ADDR_TYPE> ADDR_TYPE MemoryMappedPayload<ADDR_TYPE>::getAddress() const {
	return mDestinationAddress;
}

// Modifiers
template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::setCommand(
		const MemoryMappedCommand& cmd) {
	mCommand = cmd;
}

template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::setDataPtr(
		::hv::common::hvuint8_t* dataPtr) {
	mData = dataPtr;
}

template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::setDataLength(
		const ::std::size_t& dataLen) {
	mLength = dataLen;
}

template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::setResponseStatus(
		const MemoryMappedResponseStatus& responseStatus) {
	mResponseStatus = responseStatus;
}

template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::setAddress(
		const ADDR_TYPE& addr_) {
	mDestinationAddress = addr_;
}

template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::setDMIAllowed(
		const bool& dmiAllowed) {
	mDMIAllowed = dmiAllowed;
}

// Helpers
template<typename ADDR_TYPE> bool MemoryMappedPayload<ADDR_TYPE>::isResponseOk() const {
	return this->mResponseStatus;
}

template<typename ADDR_TYPE> bool MemoryMappedPayload<ADDR_TYPE>::isResponseError() const {
	return !this->mResponseStatus;
}

template<typename ADDR_TYPE> bool MemoryMappedPayload<ADDR_TYPE>::isDMIAllowed() const {
	return mDMIAllowed;
}

template<typename ADDR_TYPE> ::std::string MemoryMappedPayload<ADDR_TYPE>::getResponseString() const {
	std::string ret;
	switch (mResponseStatus) {
	case MEM_MAP_OK_RESPONSE:
		ret = "MEM_MAP_OK_RESPONSE";
		break;
	case MEM_MAP_INCOMPLETE_RESPONSE:
		ret = "TLM_INCOMPLETE_RESPONSE";
		break;
	case MEM_MAP_GENERIC_ERROR_RESPONSE:
		ret = "TLM_GENERIC_ERROR_RESPONSE";
		break;
	case MEM_MAP_ADDRESS_ERROR_RESPONSE:
		ret = "TLM_ADDRESS_ERROR_RESPONSE";
		break;
	case MEM_MAP_COMMAND_ERROR_RESPONSE:
		ret = "TLM_COMMAND_ERROR_RESPONSE";
		break;
	case MEM_MAP_BURST_ERROR_RESPONSE:
		ret = "TLM_BURST_ERROR_RESPONSE";
		break;
	default:
		ret = "TLM_BYTE_ENABLE_ERROR_RESPONSE";
		break;
	}
	return ret;
}

// Deep copy
template<typename ADDR_TYPE> bool MemoryMappedPayload<ADDR_TYPE>::deepCopyFrom(
		const MemoryMappedPayload& src) {
	mCommand = src.getCommand();
	mData = src.getDataPtr();
	mLength = src.getDataLength();
	mResponseStatus = src.getResponseStatus();
	mDestinationAddress = src.getAddress();
	// Copying extensions
	return BasePayload::deepCopyFrom(src);
}

template<typename ADDR_TYPE> void MemoryMappedPayload<ADDR_TYPE>::updateExtensionsFrom(
		const MemoryMappedPayload& src) {
	BasePayload::updateExtensionsFrom((BasePayload&) src);
}
}
}
}
}
}

