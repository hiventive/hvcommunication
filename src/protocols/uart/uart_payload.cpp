/**
 * @file uart_payload.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Payload
 *
 * UART payload
 */
#include "protocols/uart/uart_payload.h"

using namespace ::hv::communication::tlm2;
using namespace ::hv::common;

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace uart {

UartPayload::UartPayload() :
		BasePayload(), mCommand(UART_SEND_CHAR_COMMAND), mData(nullptr), mLength(
				0u), mResponseStatus(UART_OK_RESPONSE), mParityBits(nullptr) {
}

UartPayload::UartPayload(MemoryManagementIf* MM_) :
		BasePayload(MM_), mCommand(UART_SEND_CHAR_COMMAND), mData(nullptr), mLength(
				0u), mResponseStatus(UART_OK_RESPONSE), mParityBits(nullptr) {
}

UartCommand UartPayload::getCommand() const {
	return this->mCommand;
}

hvuint8_t* UartPayload::getDataPtr() const {
	return this->mData;
}

std::size_t UartPayload::getDataLength() const {
	return this->mLength;
}

UartResponseStatus UartPayload::getResponseStatus() const {
	return this->mResponseStatus;
}

bool UartPayload::getParityBit(const std::size_t &ind) const {
	HV_ASSERT(mParityBits != nullptr, "No parity bits defined")
	HV_ASSERT(ind < mLength,
			"Trying to retrieve a parity bit out of data scope")
	return mParityBits[ind];
}

bool* UartPayload::getParityBits() const {
	return mParityBits;
}

void UartPayload::setCommand(const UartCommand &cmd) {
	mCommand = cmd;
}

void UartPayload::setDataPtr(hvuint8_t* dataPtr) {
	mData = dataPtr;
}

void UartPayload::setDataLength(const std::size_t &dataLen) {
	mLength = dataLen;
}

void UartPayload::setResponseStatus(const UartResponseStatus &responseStatus) {
	mResponseStatus = responseStatus;
}

void UartPayload::setParityBits(bool *parityBits) {
	mParityBits = parityBits;
}

bool UartPayload::isResponseOk() const {
	return this->mResponseStatus > 0;
}

bool UartPayload::isResponseError() const {
	return !this->mResponseStatus;
}

::std::string UartPayload::getResponseString() const {
	std::string ret;
	switch (mResponseStatus) {
	case UART_OK_RESPONSE:
		ret = "UART_OK_RESPONSE";
		break;
	case UART_ERROR_RESPONSE:
		ret = "UART_ERROR_RESPONSE";
		break;
	}
	return ret;
}

bool UartPayload::hasParityBits() const {
	return mParityBits != nullptr;
}

bool UartPayload::deepCopyFrom(const UartPayload& src) {
	mCommand = src.getCommand();
	mData = src.getDataPtr();
	mLength = src.getDataLength();
	mResponseStatus = src.getResponseStatus();
	mParityBits = src.getParityBits();
	// Copying extensions
	return BasePayload::deepCopyFrom(src);
}

void UartPayload::updateExtensionsFrom(const UartPayload &src) {
	BasePayload::updateExtensionsFrom((BasePayload&) src);
}

}
}
}
}
}
