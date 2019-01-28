#include <HVCommunication>

#include "base-target.h"

class HVTarget : public BaseTarget
{
	typedef typename ::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes::tlm_payload_type
		transactionType;

public:
	HVTarget(::sc_core::sc_module_name name):
			BaseTarget(name),
			socket("socket")
	{
		socket.registerBTransport(this, &HVTarget::b_transport);
		socket.registerGetDirectMemPtr(this, &HVTarget::get_direct_mem_ptr);
		socket.registerTransportDbg(this, &HVTarget::transport_dbg);
	}

private:
	void b_transport(transactionType& trans, sc_core::sc_time& delay) {
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
				trans.setResponseStatus(::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_ERROR_RESPONSE);
				break;
			case 1:
				// TODO
				// trans.setResponseStatus(::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_BYTE_ENABLE_ERROR_RESPONSE);
				break;
			case 2:
				// TODO
				//trans.setResponseStatus(::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_BURST_ERROR_RESPONSE);
				break;
			case 3:
				trans.setResponseStatus(::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_OK_RESPONSE);
				break;
		}

		if(dmiAllowed) {
			trans.setDMIAllowed(true);
		}
	}

	bool get_direct_mem_ptr(transactionType& trans,
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

		trans.setDMIAllowed(dmiAllowed);
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

	unsigned int transport_dbg(transactionType& trans) {
		unsigned char command;

		switch(trans.getCommand()) {
			case ::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_READ_COMMAND:
				command = 0;
				break;
			case ::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_WRITE_COMMAND:
				command = 1;
				break;
			case ::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_IGNORE_COMMAND:
				command = 2;
				break;
		}

		unsigned char* dataPtr = trans.getDataPtr();
		unsigned int dataLength = trans.getDataLength();
		unsigned long address = trans.getAddress();

		return ::BaseTarget::transport_dbg(command, dataLength, dataPtr, address);
	}

public:
	::hv::communication::tlm2::protocols::memorymapped::MemoryMappedSimpleTargetSocket<> socket;
};

