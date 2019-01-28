/**
 * @file uart_protocol_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Sockets Tests Helpers
 */
#ifndef TESTS_UART_PROTOCOL_TEST_CLASSES_H_
#define TESTS_UART_PROTOCOL_TEST_CLASSES_H_

#include "protocols/uart/uart_socket.h"

SC_MODULE(UartTestModule), public ::hv::communication::tlm2::FwBwTransportIf<
		::hv::communication::tlm2::protocols::uart::UartProtocolTypes> {
	typedef FwBwTransportIf<::hv::communication::tlm2::protocols::uart::UartProtocolTypes> UartFwBwTransportIf;
public:
	SC_CTOR(UartTestModule) :
			socket("Socket") {
		socket((UartFwBwTransportIf&) *this);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::protocols::uart::UartSocket<> socket;

protected:
	void myThread() {
		::sc_core::sc_time t = ::sc_core::SC_ZERO_TIME;
		::hv::common::hvuint8_t data[10];
		for (::hv::common::hvuint32_t i = 0; i < 10; i++) {
			::hv::communication::tlm2::protocols::uart::UartPayload txn;
			std::size_t len = rand() % 5 + 5;
			std::cout << ::sc_core::sc_time_stamp() << " - " << name() << ": Sending "
					<< len << " characters through UART" << std::endl;
			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Setting char command" << std::endl;
			txn.setCommand(::hv::communication::tlm2::protocols::uart::UartCommand::UART_SEND_CHAR_COMMAND);
			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Setting data length to " << len << std::endl;
			txn.setDataLength(len);

			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Character values:" << std::endl;
			for (::hv::common::hvuint32_t j = 0; j < len; j++) {
				::hv::common::hvuint32_t valTmp(rand() % 256);
				data[j] = static_cast<::hv::common::hvuint8_t>(valTmp);
				std::cout << valTmp << "\t";
			}
			txn.setDataPtr(&data[0]);
			std::cout << std::endl;
			::hv::common::hvuint32_t timeWaitMS(rand() % 10 + 1);
			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Sending b_transport..." << std::endl;
			socket->b_transport(txn, t);
			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Checking response status..." << std::endl;
			if (txn.getResponseStatus()
					== ::hv::communication::tlm2::protocols::uart::UartResponseStatus::UART_OK_RESPONSE) {
				std::cout << ::sc_core::sc_time_stamp() << " - " << name()
						<< ": Response OK!" << std::endl;
			} else {
				std::cout << ::sc_core::sc_time_stamp() << " - " << name()
						<< ": Response not OK!" << std::endl;
				exit(EXIT_FAILURE);
			}
			std::cout << ::sc_core::sc_time_stamp() << " - " << name() << ": Waiting for "
					<< timeWaitMS << " ms" << std::endl;
			::sc_core::wait(timeWaitMS, ::sc_core::SC_MS);
		}
		::hv::communication::tlm2::protocols::uart::UartPayload txnEnd;
		std::cout << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Setting break command" << std::endl;
		txnEnd.setCommand(::hv::communication::tlm2::protocols::uart::UartCommand::UART_BREAK_CHAR_COMMAND);
		std::cout << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Sending b_transport..." << std::endl;
		socket->b_transport(txnEnd, t);
		std::cout << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Checking response status..." << std::endl;
		if (txnEnd.getResponseStatus()
				== ::hv::communication::tlm2::protocols::uart::UartResponseStatus::UART_OK_RESPONSE) {
			std::cout << ::sc_core::sc_time_stamp() << " - " << name() << ": Response OK!"
					<< std::endl;
		} else {
			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Response not OK!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	//** BW Interface **//
	tlm::tlm_sync_enum nb_transport_bw(::hv::communication::tlm2::protocols::uart::UartPayload& tran, tlm::tlm_phase& phase,
			sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}

	//** FW Interface **//
	tlm::tlm_sync_enum nb_transport_fw(::hv::communication::tlm2::protocols::uart::UartPayload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(::hv::communication::tlm2::protocols::uart::UartPayload& trans, tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(::hv::communication::tlm2::protocols::uart::UartPayload& trans) {
		return 0;
	}

	void b_transport(::hv::communication::tlm2::protocols::uart::UartPayload& trans, sc_core::sc_time& t) {
		std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Entering b_transport" << std::endl;
		if (trans.getCommand() == ::hv::communication::tlm2::protocols::uart::UartCommand::UART_SEND_CHAR_COMMAND) {
			std::size_t len = trans.getDataLength();
			std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Received " << len << " characters" << std::endl;
			::hv::common::hvuint8_t* data = trans.getDataPtr();
			std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Character values:" << std::endl;

			for (::hv::common::hvuint32_t i = 0; i < len; i++) {
				std::cout << "\t" << static_cast<::hv::common::hvuint32_t>(data[i]);
			}
			std::cout << std::endl;

		} else {
			std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Received break command" << std::endl;
		}

		std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Setting OK response..." << std::endl;
		trans.setResponseStatus(::hv::communication::tlm2::protocols::uart::UartResponseStatus::UART_OK_RESPONSE);

		std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Exiting b_transport" << std::endl;
		return;
	}

};

#endif
