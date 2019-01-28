#include "base-target.h"

class TLMTarget : public BaseTarget
{
public:
	TLMTarget(::sc_core::sc_module_name name):
			BaseTarget(name),
			socket("socket")
	{
		socket.register_b_transport(this, &TLMTarget::b_transport);
		socket.register_get_direct_mem_ptr(this, &TLMTarget::get_direct_mem_ptr);
		socket.register_transport_dbg(this, &TLMTarget::transport_dbg);
	}

protected:
	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
		unsigned char command;
		unsigned int dataLength;
		unsigned char* data;
		unsigned long address;
		unsigned char* byteEnable;
		unsigned int streamingWidth;
		bool dmiAllowed;
		unsigned char responseStatus;

		::BaseTarget::b_transport(command, dataLength, data, address, byteEnable, streamingWidth, dmiAllowed,
								  responseStatus, delay);

		switch(responseStatus) {
			case 0:
				trans.set_response_status(::tlm::TLM_ADDRESS_ERROR_RESPONSE);
				break;
			case 1:
				trans.set_response_status(::tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
				break;
			case 2:
				trans.set_response_status(::tlm::TLM_BURST_ERROR_RESPONSE);
				break;
			case 3:
				trans.set_response_status(::tlm::TLM_OK_RESPONSE);
				break;
		}

		if(dmiAllowed) {
			trans.set_dmi_allowed(true);
		}
	}

	bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
							tlm::tlm_dmi& dmiData) {
		bool dmiAllowed;
		bool allowReadWrite;
		unsigned char* dmiPtr;
		unsigned long startAddress;
		unsigned long endAddress;
		::sc_core::sc_time readLatency;
		::sc_core::sc_time writeLatency;

		bool result = ::BaseTarget::get_direct_mem_ptr(dmiAllowed, allowReadWrite, dmiPtr,
													   startAddress, endAddress, readLatency, writeLatency);

		trans.set_dmi_allowed(dmiAllowed);
		if(allowReadWrite) {
			dmiData.allow_read_write();
		}
		dmiData.set_dmi_ptr(dmiPtr);
		dmiData.set_start_address(startAddress);
		dmiData.set_end_address(endAddress);
		dmiData.set_read_latency(readLatency);
		dmiData.set_write_latency(writeLatency);

		return result;
	}

	unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
		unsigned char command;

		switch(trans.get_command()) {
			case ::tlm::TLM_READ_COMMAND:
				command = 0;
				break;
			case ::tlm::TLM_WRITE_COMMAND:
				command = 1;
				break;
			case ::tlm::TLM_IGNORE_COMMAND:
				command = 2;
				break;
		}

		unsigned char* dataPtr = trans.get_data_ptr();
		unsigned int dataLength = trans.get_data_length();
		unsigned long address = trans.get_address();

		return ::BaseTarget::transport_dbg(command, dataLength, dataPtr, address);
	}

public:
	::tlm_utils::simple_target_socket<TLMTarget> socket;
};

