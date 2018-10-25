#include <iostream>
#include <systemc>
#include <hv/communication.h>

int sc_main(int argc, char* argv[]) {
	::hv::communication::tlm2::protocols::uart::UartPayload payload;
	payload.setCommand(::hv::communication::tlm2::protocols::uart::UART_BREAK_CHAR_COMMAND);
	std::cout << payload.getCommand() << std::endl;
	return 0;
}
