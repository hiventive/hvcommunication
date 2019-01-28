#include "base-initiator.h"

class TLMInitiator : public BaseInitiator
{
	SC_HAS_PROCESS(TLMInitiator);
public:
	TLMInitiator(::sc_core::sc_module_name name):
		BaseInitiator(name) {
		socket.register_invalidate_direct_mem_ptr(this,
				&TLMInitiator::invalidate_direct_mem_ptr);
		SC_THREAD(example);
	}

private:
	void example() {
		::tlm::tlm_generic_payload payload;
		::sc_core::sc_time delay(::sc_core::SC_ZERO_TIME);

		unsigned char data = 'H';

		payload.set_command(tlm::TLM_WRITE_COMMAND);
		payload.set_address(0xFF);
		payload.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
		payload.set_data_length(4);
		payload.set_streaming_width(4);
		payload.set_byte_enable_ptr(0);
		payload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

		socket->b_transport(payload, delay);

		if (payload.is_response_error()) {
			char txt[100];
			sprintf(txt, "Error from b_transport, response status = %s",
					payload.get_response_string().c_str());
			SC_REPORT_ERROR("TLM-2", txt);
		}

		if (payload.is_dmi_allowed()) {
			payload.set_address(0x000000FF);
			dmiPtrValid = socket->get_direct_mem_ptr(payload, dmiData);
		}

		std::cout << "Done" << std::endl;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 startRange,
								   sc_dt::uint64 endRange) {
		::BaseInitiator::invalidate_direct_mem_ptr(startRange, endRange);
	}

public:
	::tlm_utils::simple_initiator_socket<TLMInitiator> socket;
};
