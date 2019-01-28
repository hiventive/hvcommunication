/**
 * @file irqs_protocol_test.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM IRQ Sockets Tests
 */



#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>
#include <HVCommon>
#include "protocols/irq/irq_socket.h"

#include "irq_protocol_test_classes.h"

using namespace ::hv::communication::tlm2;
using namespace sc_core;

class IRQSocketTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

};


TEST_F(IRQSocketTest, BindingAndUsageTest) {
	HV_SYSTEMC_RESET_CONTEXT
	IRQTestModuleInit module1("Module1", 1), module2("Module2", 2), module3("Module3", 3);
	IRQTestModuleTarget moduleTarget("TargetModule");
	module1.socket(moduleTarget.socket);
	module2.socket(moduleTarget.socket);
	module3.socket(moduleTarget.socket);

	sc_core::sc_start();
}
