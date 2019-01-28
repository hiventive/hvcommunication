/**
 * @file memory_mapped_protocol converters.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory-Mapped Convenient Socket
 *
 * Memory-Mapped Protocol Converters
 */

#ifndef SRC_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PROTOCOL_CONVERTERS_H_
#define SRC_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PROTOCOL_CONVERTERS_H_

#include <tlm>
#include "core/tlm-2/protocol_adapter/protocol_converter_if.h"
#include "protocols/memory_mapped/memory_mapped_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace memorymapped {

/**
 * This structure provides protocol conversion from/to
 * FIXME Copy extensions too
 */
struct TLMGenericMemoryMapProtocolConverter: public ProtocolConverterIf<
		::tlm::tlm_base_protocol_types, MemoryMappedProtocolTypes> {

	virtual ~TLMGenericMemoryMapProtocolConverter() {
	}

	virtual void deepCopy(const payload_in_type& src,
			payload_out_type& dst) override {
		// Converting command
		::tlm::tlm_command cmd = src.get_command();
		switch (cmd) {
		case ::tlm::TLM_READ_COMMAND:
			dst.setCommand(MEM_MAP_READ_COMMAND);
			break;
		case ::tlm::TLM_WRITE_COMMAND:
			dst.setCommand(MEM_MAP_WRITE_COMMAND);
			break;
		default:
			dst.setCommand(MEM_MAP_IGNORE_COMMAND);
			break;
		}
		// Converting data ptr
		dst.setDataPtr(src.get_data_ptr());
		// Converting data length
		dst.setDataLength(src.get_data_length());
		// Converting destination address
		dst.setAddress(src.get_address());
		// Converting response status
		::tlm::tlm_response_status rs = src.get_response_status();
		switch (rs) {
		case ::tlm::TLM_OK_RESPONSE:
			dst.setResponseStatus(MEM_MAP_OK_RESPONSE);
			break;
		case ::tlm::TLM_INCOMPLETE_RESPONSE:
			dst.setResponseStatus(MEM_MAP_INCOMPLETE_RESPONSE);
			break;
		case ::tlm::TLM_GENERIC_ERROR_RESPONSE:
			dst.setResponseStatus(MEM_MAP_GENERIC_ERROR_RESPONSE);
			break;
		case ::tlm::TLM_ADDRESS_ERROR_RESPONSE:
			dst.setResponseStatus(MEM_MAP_ADDRESS_ERROR_RESPONSE);
			break;
		case ::tlm::TLM_COMMAND_ERROR_RESPONSE:
			dst.setResponseStatus(MEM_MAP_COMMAND_ERROR_RESPONSE);
			break;
		case ::tlm::TLM_BURST_ERROR_RESPONSE:
			dst.setResponseStatus(MEM_MAP_BURST_ERROR_RESPONSE);
			break;
		case ::tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE:
			dst.setResponseStatus(MEM_MAP_BYTE_ENABLE_ERROR_RESPONSE);
			break;
		default:
			HV_WARN("Unsupported response")
			dst.setResponseStatus(MEM_MAP_GENERIC_ERROR_RESPONSE);
			break;
		}
		// Converting DMI allowed
		dst.setDMIAllowed(src.is_dmi_allowed());
	}

	virtual void deepCopy(const payload_out_type& src,
			payload_in_type& dst) override {
		// Converting command
		MemoryMappedCommand cmd = src.getCommand();
		dst.set_command(
				cmd == MEM_MAP_READ_COMMAND ? ::tlm::TLM_READ_COMMAND :
				cmd == MEM_MAP_WRITE_COMMAND ?
						::tlm::TLM_WRITE_COMMAND : ::tlm::TLM_IGNORE_COMMAND);
		// Converting data ptr
		dst.set_data_ptr(src.getDataPtr());
		// Converting data length
		dst.set_data_length(src.getDataLength());
		// Converting destination address
		dst.set_address(src.getAddress());
		// Converting response status
		MemoryMappedResponseStatus status = src.getResponseStatus();
		switch (status) {
		case MEM_MAP_OK_RESPONSE:
			dst.set_response_status(::tlm::TLM_OK_RESPONSE);
			break;
		case MEM_MAP_INCOMPLETE_RESPONSE:
			dst.set_response_status(::tlm::TLM_INCOMPLETE_RESPONSE);
			break;
		case MEM_MAP_GENERIC_ERROR_RESPONSE:
			dst.set_response_status(::tlm::TLM_GENERIC_ERROR_RESPONSE);
			break;
		case MEM_MAP_ADDRESS_ERROR_RESPONSE:
			dst.set_response_status(::tlm::TLM_ADDRESS_ERROR_RESPONSE);
			break;
		case MEM_MAP_COMMAND_ERROR_RESPONSE:
			dst.set_response_status(::tlm::TLM_COMMAND_ERROR_RESPONSE);
			break;
		case MEM_MAP_BURST_ERROR_RESPONSE:
			dst.set_response_status(::tlm::TLM_BURST_ERROR_RESPONSE);
			break;
		case MEM_MAP_BYTE_ENABLE_ERROR_RESPONSE:
			dst.set_response_status(::tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
			break;
		default:
			HV_WARN("Unsupported response")
			dst.set_response_status(::tlm::TLM_GENERIC_ERROR_RESPONSE);
			break;
		}

		// Converting DMI allowed
		dst.set_dmi_allowed(src.isDMIAllowed());
	}

};

}
}
}
}
}

#endif /* SRC_PROTOCOLS_MEMORY_MAPPED_MEMORY_MAPPED_PROTOCOL_CONVERTERS_H_ */
