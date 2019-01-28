/**
 * @file signal_protocol_test.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Sockets Tests
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>
#include <HVCommon>
#include "protocols/signal/signal_socket.h"

#include "signal_protocol_test_classes.h"

using namespace ::hv::communication::tlm2;
using namespace sc_core;

class SignalSocketTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

};

TEST_F(SignalSocketTest,GeneralTest) {
	HV_SYSTEMC_RESET_CONTEXT
	SignalSocketTestInitiatorModule initModule("SignalInitModule");
	SignalSocketTestTargetModule targModule("SignalTargModule");
	initModule.socket(targModule.socket);

	sc_core::sc_start();

	for (auto i = 0; i < N_SIG_TESTS; i++) {
		ASSERT_EQ(sigIn[i], sigOut[i]);
	}
}
