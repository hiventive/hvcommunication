/**
 * @file irq_payload.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM IRQ Payload
 *
 * IRQ payload
 */

#include "irq_payload.h"

using namespace ::hv::common;

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace irq {

IRQPayload::IRQPayload() :
		BasePayload(), mID(0u), mValue(false), mResponseStatus(IRQ_OK_RESPONSE) {

}

IRQPayload::IRQPayload(MemoryManagementIf* MM_) :
		BasePayload(MM_), mID(0u), mValue(false), mResponseStatus(
				IRQ_OK_RESPONSE) {

}

hvuint32_t IRQPayload::getID() const {
	return mID;
}

bool IRQPayload::getValue() const {
	return mValue;
}

IRQResponseStatus IRQPayload::getResponseStatus() const {
	return mResponseStatus;
}

void IRQPayload::setID(const hvuint32_t &ID_) {
	mID = ID_;
}

void IRQPayload::setValue(const bool &value_) {
	mValue = value_;
}

void IRQPayload::setResponseStatus(const IRQResponseStatus &responseStatus_) {
	mResponseStatus = responseStatus_;
}

bool IRQPayload::isResponseOk() const {
	return mResponseStatus;
}

bool IRQPayload::isResponseError() const {
	return !mResponseStatus;
}

::std::string IRQPayload::getResponseString() const {
	std::string ret;
	switch(mResponseStatus) {
	case IRQ_OK_RESPONSE:
		ret = "IRQ_OK_RESPONSE";
		break;
	default:
		ret = "IRQ_ERROR_RESPONSE";
		break;
	}
	return ret;
}

bool IRQPayload::deepCopyFrom(const IRQPayload & src) {
	mID = src.getID();
	mResponseStatus = src.getResponseStatus();
	return BasePayload::deepCopyFrom(src);
}

void IRQPayload::updateExtensionsFrom(const IRQPayload &src) {
	BasePayload::updateExtensionsFrom((BasePayload&) src);
}

}
}
}
}
}
