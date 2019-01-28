/**
 * @file uart_payload.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Payload
 *
 * UART payload
 *
 * Note: if mParityBits is equal to nullptr, then parity is not activated
 */

#ifndef HV_PROTOCOLS_UART_PAYLOAD_H_
#define HV_PROTOCOLS_UART_PAYLOAD_H_

#include <tlm>
#include <core/tlm-2/base_payload/base_payload.h>

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace uart {

enum UartCommand {
	UART_SEND_CHAR_COMMAND, UART_BREAK_CHAR_COMMAND
};

enum UartResponseStatus {
	UART_OK_RESPONSE = 1, UART_ERROR_RESPONSE = 0
};

class UartPayload: public BasePayload {
public:
	// Constructors
	UartPayload();
	explicit UartPayload(MemoryManagementIf* MM_);

	// Destructor
	virtual ~UartPayload() {
	}

	// Getters
	UartCommand getCommand() const;
	::hv::common::hvuint8_t* getDataPtr() const;
	std::size_t getDataLength() const;
	UartResponseStatus getResponseStatus() const;
	bool getParityBit(const std::size_t &ind) const;
	bool* getParityBits() const;

	// Modifiers
	void setCommand(const UartCommand &cmd);
	void setDataPtr(::hv::common::hvuint8_t* dataPtr);
	void setDataLength(const std::size_t& dataLen);
	void setResponseStatus(const UartResponseStatus& responseStatus);
	void setParityBits(bool* parityBits);

	// Helpers
	bool isResponseOk() const;
	bool isResponseError() const;
	bool hasParityBits() const;

	// Deep copy
	bool deepCopyFrom(const UartPayload& src);
	void updateExtensionsFrom(const UartPayload& src);
	::std::string getResponseString() const;

private:
	// Deleting copy ctor and assignment operator
	// Note: they are set private and non-implemented instead for compatibility
	UartPayload(const UartPayload&) /* = delete */;
	UartPayload& operator =(const UartPayload& src) /* = delete */;

protected:
	UartCommand mCommand;
	::hv::common::hvuint8_t* mData;
	std::size_t mLength;
	UartResponseStatus mResponseStatus;
	bool* mParityBits;
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_UART_PAYLOAD_H_ */
