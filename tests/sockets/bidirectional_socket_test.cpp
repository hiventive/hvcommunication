/**
 * @file bidirectional_socket_test.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Bidirectional Socket Tests
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>
#include <HVCommon>
#include "core/tlm-2/sockets/bidirectional_socket.h"
#include "bidirectional_socket_test_classes.h"

using namespace ::hv::communication::tlm2;
using namespace sc_core;

class BidirectionalSocketTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}

};

//// Bidirectional socket binding test
TEST_F(BidirectionalSocketTest, BindingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	TestModule A("A");
	TestModule B("B");
	std::cout << sc_core::sc_time_stamp() << ": Binding A and B" << std::endl;
	A.socket(B.socket);
	sc_core::sc_start();
}

// Tripartite binding
TEST_F(BidirectionalSocketTest, TripartiteBindingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	TestModuleInit A("Initiator");
	TestModuleBidir B("Transactor");
	TestModuleTarget C("Target");

	A.socket.bind(B.socket);
	B.socket.bind(C.socket);

	sc_core::sc_start();
}
