/**
 * @file convenient_socket_test.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Convenient Sockets Tests
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>
#include <HVCommon>
#include "convenient_sockets_test_classes.h"

using namespace ::hv::communication::tlm2;
using namespace sc_core;

class ConvenientSocketTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}

};

TEST_F(ConvenientSocketTest, SimpleTest) {
	HV_SYSTEMC_RESET_CONTEXT
	SimpleInitSocketTestModule initModule("InitModule");
	SimpleTargetSocketTestModule targetModule("TargetModule");
	initModule.socket(targetModule.socket);

	sc_core::sc_start();
}

TEST_F(ConvenientSocketTest, HierarchicalBindingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	SimpleInitSocketHierarchicalBindingTestModule initModule("InitModuleBinding");
	SimpleTargetSocketHierarchicalBindingTestModule targModule("TargModuleBinding");
	initModule.socket(targModule.socket);

	sc_core::sc_start();
}

TEST_F(ConvenientSocketTest, SimpleBidirectionalSocketTest) {
	HV_SYSTEMC_RESET_CONTEXT
	SimpleBidirectionalSocketTestModule mod1("BidMod1"), mod2("BidMod2");
	mod1.socket(mod2.socket);

	sc_core::sc_start();
}

TEST_F(ConvenientSocketTest, SimpleBidirectionalSocketHierarchicalBindingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	SimpleBidirectionalSocketHierarchicalBindingTestModule mod1("BidMod1"), mod2("BidMod2");
	mod1.socket(mod2.socket);

	sc_core::sc_start();
}
