/**
 * @file uart_protocol_test.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Sockets Tests
 */



#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>
#include <HVCommon>
#include "protocols/uart/uart_socket.h"

#include "uart_protocol_test_classes.h"

using namespace ::hv::communication::tlm2;
using namespace sc_core;

class UartSocketTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

};


TEST_F(UartSocketTest, BindingAndUsageTest) {
	HV_SYSTEMC_RESET_CONTEXT
	UartTestModule UART1("UART1"), UART2("UART2");
	UART1.socket(UART2.socket);


	sc_core::sc_start();
}
