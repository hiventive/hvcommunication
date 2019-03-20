/**
 * @file signal_payload.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Payload
 *
 * Signal Payload
 */

#ifndef HV_PROTOCOLS_SIGNAL_SIGNAL_PAYLOAD_H_
#define HV_PROTOCOLS_SIGNAL_SIGNAL_PAYLOAD_H_

#include <tlm>

#include "../../core/tlm-2/base_payload/base_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace signal {

// FIXME: move to common?
typedef ::hv::common::hvuint8_t hv_signal_t;

enum SignalState : hv_signal_t { SIGNAL_DOWN = 0, SIGNAL_UP = 1, SIGNAL_UNSET = 2 };

enum SignalResponseStatus { SIGNAL_OK_RESPONSE = 1, SIGNAL_ERROR_RESPONSE = 0 };

class SignalPayload : public BasePayload {
  public:
    SignalPayload();
    explicit SignalPayload(MemoryManagementIf *);

    virtual ~SignalPayload();

    // Accessors
    hv_signal_t getValue() const;
    SignalResponseStatus getResponseStatus() const;

    // Modifiers
    void setValue(const hv_signal_t &);
    void setValue(const bool &);
    void setResponseStatus(const SignalResponseStatus &);

    // Helpers
    bool isResponseOk() const;
    bool isResponseError() const;
    ::std::string getResponseString() const override;

  private:
    // Deleting copy ctor and assignment operator
    // Note: they are set private and non-implemented instead for compatibility
    SignalPayload(const SignalPayload &) /* = delete */;
    SignalPayload &operator=(const SignalPayload &src) /* = delete */;

  protected:
    hv_signal_t mSignalValue;
    SignalResponseStatus mResponseStatus;
};

} // namespace signal
} // namespace protocols
} // namespace tlm2
} // namespace communication
} // namespace hv

#endif /* HV_PROTOCOLS_SIGNAL_SIGNAL_PAYLOAD_H_ */
