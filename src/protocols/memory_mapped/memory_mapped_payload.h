/**
 * @file memory_mapped_payload.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory-Mapped Payload
 *
 * Memory-mapped payload
 */

#ifndef HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PAYLOAD_H_
#define HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PAYLOAD_H_

#include <tlm_core/tlm_2/tlm_generic_payload/tlm_generic_payload.h>

#include "../../core/tlm-2/base_payload/base_payload.h"
#include "../../location/addressing_payload_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace memorymapped {

enum MemoryMappedCommand {
	MEM_MAP_READ_COMMAND, MEM_MAP_WRITE_COMMAND, MEM_MAP_IGNORE_COMMAND
};

enum MemoryMappedResponseStatus {
	MEM_MAP_OK_RESPONSE = 1,
	MEM_MAP_INCOMPLETE_RESPONSE = 0,
	MEM_MAP_GENERIC_ERROR_RESPONSE = -1,
	MEM_MAP_ADDRESS_ERROR_RESPONSE = -2,
	MEM_MAP_COMMAND_ERROR_RESPONSE = -3,
	MEM_MAP_BURST_ERROR_RESPONSE = -4,
	MEM_MAP_BYTE_ENABLE_ERROR_RESPONSE = -5
};

template<typename ADDR_TYPE> class MemoryMappedPayload: public BasePayload,
		public AddressingPayloadIf<ADDR_TYPE> {
public:
	typedef ADDR_TYPE address_type;

	MemoryMappedPayload();
	explicit MemoryMappedPayload(MemoryManagementIf* MM_);

	virtual ~MemoryMappedPayload();

	// Accessors
	MemoryMappedCommand getCommand() const;
	std::string getCommandStr() const;
	::hv::common::hvuint8_t* getDataPtr() const;
	::std::size_t getDataLength() const;
	MemoryMappedResponseStatus getResponseStatus() const;
	address_type getAddress() const;

	// Modifiers
	void setCommand(const MemoryMappedCommand& cmd);
	void setDataPtr(::hv::common::hvuint8_t* dataPtr);
	void setDataLength(const ::std::size_t& dataLen);
	void setResponseStatus(const MemoryMappedResponseStatus& responseStatus);
	void setAddress(const address_type& destAddr);
	void setDMIAllowed(const bool& dmiAllowed);

	// Helpers
	bool isResponseOk() const;
	bool isResponseError() const;
	bool isDMIAllowed() const;
	::std::string getResponseString() const;

	// Deep copy
	bool deepCopyFrom(const MemoryMappedPayload& src);
	void updateExtensionsFrom(const MemoryMappedPayload& src);

private:
	MemoryMappedPayload(const MemoryMappedPayload&) /* = delete */;
	MemoryMappedPayload& operator =(
			const MemoryMappedPayload& src) /* = delete */;

protected:
	MemoryMappedCommand mCommand;
	::hv::common::hvuint8_t* mData;
	::std::size_t mLength;
	MemoryMappedResponseStatus mResponseStatus;
	address_type mDestinationAddress;
	bool mDMIAllowed;
};

}
}
}
}
}

#include "memory_mapped_payload.hpp"

#endif /* HV_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PAYLOAD_H_ */
