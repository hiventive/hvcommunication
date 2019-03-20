/**
 * @file signal_payload.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Payload
 *
 * Signal Payload
 */

#include "signal_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace signal {

SignalPayload::SignalPayload()
    : BasePayload(), mSignalValue(false), mResponseStatus(SIGNAL_OK_RESPONSE) {
}

SignalPayload::SignalPayload(MemoryManagementIf *mm_)
    : BasePayload(mm_), mSignalValue(false), mResponseStatus(SIGNAL_OK_RESPONSE) {
}

SignalPayload::~SignalPayload() {
}

// Accessors
hv_signal_t SignalPayload::getValue() const {
    return mSignalValue;
}

SignalResponseStatus SignalPayload::getResponseStatus() const {
    return mResponseStatus;
}

// Modifiers
void SignalPayload::setValue(const hv_signal_t &sigVal_) {
    mSignalValue = sigVal_;
}

void SignalPayload::setValue(const bool &sigVal_) {
    mSignalValue = sigVal_ ? 1 : 0;
}

void SignalPayload::setResponseStatus(const SignalResponseStatus &responseStatus_) {
    mResponseStatus = responseStatus_;
}

// Helpers
bool SignalPayload::isResponseOk() const {
    return this->mResponseStatus > 0;
}

bool SignalPayload::isResponseError() const {
    return !this->mResponseStatus;
}

::std::string SignalPayload::getResponseString() const {
    std::string ret;
    switch (mResponseStatus) {
    case SIGNAL_OK_RESPONSE:
        ret = "SIGNAL_OK_RESPONSE";
        break;
    default:
        ret = "SIGNAL_ERROR_RESPONSE";
        break;
    }
    return ret;
}

} // namespace signal
} // namespace protocols
} // namespace tlm2
} // namespace communication
} // namespace hv
