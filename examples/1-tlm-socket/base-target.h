#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

#ifndef BASE_TARGET_H
#define BASE_TARGET_H

class BaseTarget : public ::sc_core::sc_module
{
	SC_HAS_PROCESS(BaseTarget);

public:
	BaseTarget(::sc_core::sc_module_name name):
			sc_module(name),
			size(1234)
	{
		for (int i = 0; i < size; i++) {
			mem[i] = (rand() % size);
		}
	}

protected:
	void b_transport(unsigned char& command, unsigned int& dataLength, unsigned char*& data,
			unsigned long& address, unsigned char*& byteEnable, unsigned int& streamingWidth,
			bool& dmiAllowed, unsigned char& responseStatus, sc_core::sc_time& delay) {

		// responseStatus
		// ::tlm::TLM_ADDRESS_ERROR_RESPONSE = 0
		// ::tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE = 1
		// ::tlm::TLM_BURST_ERROR_RESPONSE = 2
		// ::tlm::TLM_OK_RESPONSE = 3

		if (address >= size) {
			responseStatus = 0;
			return;
		}
		if (byteEnable != 0) {
			responseStatus = 1;
			return;
		}
		if (dataLength > 4 || streamingWidth < dataLength) {
			responseStatus = 2;
			return;
		}

		wait(delay);

		if (command == ::tlm::TLM_READ_COMMAND) {
			memcpy(data, &mem[address], dataLength);
		} else if (command == ::tlm::TLM_WRITE_COMMAND) {
			memcpy(&mem[address], data, dataLength);
		}

		dmiAllowed = true;

		responseStatus = 3;
	}

	bool get_direct_mem_ptr(bool& dmiAllowed, bool& allowReadWrite, unsigned char*& dmiPtr,
			unsigned long& startAddress, unsigned long& endAddress, ::sc_core::sc_time& readLatency,
			::sc_core::sc_time& writeLatency) {
		dmiAllowed = true;
		allowReadWrite = true;
		dmiPtr = reinterpret_cast<unsigned char*>(&mem[0]);
		startAddress = 0;
		endAddress = (size * 4) - 1;
		readLatency = ::sc_core::SC_ZERO_TIME;
		writeLatency = ::sc_core::SC_ZERO_TIME;
		return true;
	}

	unsigned int transport_dbg(unsigned char& command, unsigned int& dataLength,
			unsigned char*& data, unsigned long& address) {
		// command = 0 = READ
		// command = 1 = WRITE
		// command = X = UNKNOWN_COMMAND
		unsigned int result = (dataLength < (size - address) * 4) ?
								 dataLength : (size - address) * 4;

		if (command == 0) {
			memcpy(data, &mem[address], result);
		} else if (command == 1) {
			memcpy(&mem[address], data, result);
		}

		return result;
	}

private:
	unsigned char mem[1234];
	unsigned int size;
};

#endif // BASE_TARGET_H
