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

#ifndef HV_PROTOCOLS_IRQ_PAYLOAD_H_
#define HV_PROTOCOLS_IRQ_PAYLOAD_H_

#include <core/tlm-2/base_payload/base_payload.h>
#include <tlm>


namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace irq {

enum IRQResponseStatus {
	IRQ_OK_RESPONSE = 1, IRQ_ERROR_RESPONSE = 0
};

class IRQPayload: public BasePayload {
public:
	// Constructors
	IRQPayload();
	explicit IRQPayload(MemoryManagementIf* MM_);

	// Destructor
	virtual ~IRQPayload() {
	}

	// Getters
	::hv::common::hvuint32_t getID() const;
	bool getValue() const;
	IRQResponseStatus getResponseStatus() const;

	// Modifiers
	void setID(const ::hv::common::hvuint32_t &ID_);
	void setValue(const bool &value_);
	void setResponseStatus(const IRQResponseStatus &responseStatus_);

	// Helpers
	bool isResponseOk() const;
	bool isResponseError() const;

	// Deep copy
	bool deepCopyFrom(const IRQPayload & src);
	void updateExtensionsFrom(const IRQPayload &src);
	::std::string getResponseString() const;

private:
	// Deleting copy ctor and assignment operator
	// Note: they are set private and non-implemented instead for compatibility
	IRQPayload(const IRQPayload&) /* = delete */;
	IRQPayload& operator =(const IRQPayload& src) /* = delete */;

protected:
	::hv::common::hvuint32_t mID;
	bool mValue;
	IRQResponseStatus mResponseStatus;
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_IRQ_PAYLOAD_H_ */
