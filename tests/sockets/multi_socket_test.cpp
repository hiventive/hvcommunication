/**
 * @file multi_socket_test.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Utils Multi Socket Test
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>
#include <HVCommon>
#include "multi_socket_test_classes.h"

using namespace ::hv::communication::tlm2;
using namespace ::hv::communication::tlm2::utils;
using namespace sc_core;

class MultiSocketTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

};

TEST_F(MultiSocketTest, MultiInitiatorSocketBindToSeveralTargetsTest) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "Instantiating MultiInitModule" << std::endl;
	MultiInitModule myMultiInitModule("MyMultiInitModule");
	std::cout << "Instantiating TargetModules" << std::endl;
	TargetModule myTargetModule0("MyTargetModule0"), myTargetModule1(
			"MyTargetModule1"), myTargetModule2("MyTargetModule2"),
			myTargetModule3("MyTargetModule3");

	std::cout << "Binding..." << std::endl;
	myMultiInitModule.socket.bind(myTargetModule0.socket);
	myMultiInitModule.socket.bind(myTargetModule1.socket);
	myMultiInitModule.socket.bind(myTargetModule2.socket);
	myMultiInitModule.socket.bind(myTargetModule3.socket);

	std::cout << "Starting simulation" << std::endl;
	sc_core::sc_start();
}

TEST_F(MultiSocketTest, MultiInitiatorSocketHierarchicalBindingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "Instantiating MultiInitEncapsulationModule" << std::endl;
	MultiInitEncapsulatingModule myMultiInitEncapsulatingModule(
			"MyMultiInitEncapsulatingModule");
	std::cout << "Instantiating TargetModules" << std::endl;
	TargetModule myTargetModule0("MyTargetModule0"), myTargetModule1(
			"MyTargetModule1"), myTargetModule2("MyTargetModule2"),
			myTargetModule3("MyTargetModule3");

	std::cout << "Binding..." << std::endl;
	myMultiInitEncapsulatingModule.socket.bind(myTargetModule0.socket);
	myMultiInitEncapsulatingModule.socket.bind(myTargetModule1.socket);
	myMultiInitEncapsulatingModule.socket.bind(myTargetModule2.socket);
	myMultiInitEncapsulatingModule.socket.bind(myTargetModule3.socket);

	std::cout << "Starting simulation" << std::endl;
	sc_core::sc_start();
}

TEST_F(MultiSocketTest, MultiTargetSocketBindToSeveralInitiatorsTest) {
	HV_SYSTEMC_RESET_CONTEXT

	std::cout << "Instantiating Init Modules" << std::endl;
	InitModule mInitModule0("InitModule0"), mInitModule1("InitModule1"),
			mInitModule2("InitModule2"), mInitModule3("InitModule3");

	std::cout << "Instantiating Multi Target" << std::endl;
	MultiTargetModule mMultiTargetModule("MultiTargetModule");

	std::cout << "Binding..." << std::endl;
	mInitModule0.socket.bind(mMultiTargetModule.socket);
	mInitModule1.socket.bind(mMultiTargetModule.socket);
	mInitModule2.socket.bind(mMultiTargetModule.socket);
	mInitModule3.socket.bind(mMultiTargetModule.socket);

	std::cout << "Starting simulation..." << std::endl;
	sc_core::sc_start();
}

TEST_F(MultiSocketTest, MultiTargetSocketHierarchicalBindingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "Instantiating Init Modules for Hierarchical binding test"
			<< std::endl;
	InitModule mInitModule0("InitModule0"), mInitModule1("InitModule1"),
			mInitModule2("InitModule2"), mInitModule3("InitModule3");

	std::cout << "Instantiating Encapsulating Multi Target" << std::endl;
	MultiTargetEncapsulatingModule mMultiTargetEncapsulatingModule(
			"MultiTargetEncapsulatingModule");

	std::cout << "Binding..." << std::endl;
	mInitModule0.socket.bind(mMultiTargetEncapsulatingModule.socket);
	mInitModule1.socket.bind(mMultiTargetEncapsulatingModule.socket);
	mInitModule2.socket.bind(mMultiTargetEncapsulatingModule.socket);
	mInitModule3.socket.bind(mMultiTargetEncapsulatingModule.socket);

	std::cout << "Starting simulation..." << std::endl;
	sc_core::sc_start();
}
