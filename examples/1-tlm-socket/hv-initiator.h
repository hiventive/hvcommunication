#include <HVCommunication>

#include "base-initiator.h"

class HVInitiator : public BaseInitiator
{
	SC_HAS_PROCESS(HVInitiator);
public:
	HVInitiator(::sc_core::sc_module_name name):
			BaseInitiator(name) {
		socket.registerInvalidateDirectMemPtr(this,
				&HVInitiator::invalidate_direct_mem_ptr);
		SC_THREAD(example);
	}

private:
	void example() {
		::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes::tlm_payload_type payload;
		::sc_core::sc_time delay(::sc_core::SC_ZERO_TIME);

		unsigned char data = 'H';

		payload.setCommand(::hv::communication::tlm2::protocols::memorymapped::MemoryMappedCommand::MEM_MAP_WRITE_COMMAND);
		payload.setAddress(0xFF);
		payload.setDataPtr(reinterpret_cast<unsigned char*>(&data));
		payload.setDataLength(4);
		// payload.setStreamingWidth(4); // TODO
		// payload.setByteEnablePtr(0); // TODO
		payload.setResponseStatus(::hv::communication::tlm2::protocols::memorymapped::MemoryMappedResponseStatus::MEM_MAP_INCOMPLETE_RESPONSE);

		socket->b_transport(payload, delay);

		if (payload.isResponseError()) {
			char txt[100];
			/*sprintf(txt, "Error from b_transport, response status = %s",
					payload.getResponseString().c_str());*/ // TODO
			SC_REPORT_ERROR("TLM-2", txt);
		}

		if (payload.isDMIAllowed()) {
			payload.setAddress(0x000000FF);
			dmiPtrValid = socket->get_direct_mem_ptr(payload, dmiData);
		}

		std::cout << "Done" << std::endl;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 startRange,
								   sc_dt::uint64 endRange) {
		::BaseInitiator::invalidate_direct_mem_ptr(startRange, endRange);
	}

public:
	::hv::communication::tlm2::protocols::memorymapped::MemoryMappedSimpleInitiatorSocket<> socket;
};
