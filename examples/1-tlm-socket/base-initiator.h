#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

#ifndef BASE_INITIATOR_H
#define BASE_INITIATOR_H

class BaseInitiator : public ::sc_core::sc_module
{
public:
	BaseInitiator(::sc_core::sc_module_name name):
			::sc_core::sc_module(name),
			dmiPtrValid(false) {
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 startRange,
								   sc_dt::uint64 endRange) {
		dmiPtrValid = false;
	}

protected:
	bool dmiPtrValid;
	::tlm::tlm_dmi dmiData;
};

#endif // BASE_INITIATOR_H
