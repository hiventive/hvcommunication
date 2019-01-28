/**
 * @file generic_router_test.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Generic Router Test
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>

#include "../generic_router/generic_router_test_classes.h"

class GenericRouterTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(GenericRouterTest, MemoryMappedTest) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "Instantiating init module 1" << std::endl;
	InitModuleTest initModule1("GenericRouterTestInitModule1");
	std::cout << "Instantiating init module 2" << std::endl;
	InitModuleTest initModule2("GenericRouterTestInitModule2");

	std::cout << "Instantiating target module 1" << std::endl;
	TargModuleTest targModule1("GenericRouterTestTargModule1", addresses[0], blockSize);
	std::cout << "Instantiating target module 2" << std::endl;
	TargModuleTest targModule2("GenericRouterTestTargModule2", addresses[1], blockSize);
	std::cout << "Instantiating target module 3" << std::endl;
	TargModuleTest targModule3("GenericRouterTestTargModule3", addresses[2], blockSize);
	std::cout << "Instantiating target module 4" << std::endl;
	TargModuleTest targModule4("GenericRouterTestTargModule4", addresses[3], blockSize);

	std::cout << "Instantiating router" << std::endl;
	::hv::communication::tlm2::utils::GenericRouter<> myRouter("MyRouter", true);

	std::cout << "Binding init module 1 to router" << std::endl;
	initModule1.socket.bind(myRouter.inputSocket);
	std::cout << "Binding init module 2 to router" << std::endl;
	initModule2.socket.bind(myRouter.inputSocket);

	std::cout << "Binding target module 1 to router" << std::endl;
	myRouter.outputSocket.bind(targModule1.socket);
	std::cout << "Binding target module 2 to router" << std::endl;
	myRouter.outputSocket.bind(targModule2.socket);
	std::cout << "Binding target module 3 to router" << std::endl;
	myRouter.outputSocket.bind(targModule3.socket);
	std::cout << "Binding target module 4 to router" << std::endl;
	myRouter.outputSocket.bind(targModule4.socket);

	std::cout << "Starting simulation..." << std::endl;
	::sc_core::sc_start();
}

TEST_F(GenericRouterTest, SingleAddressedTest) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "Instantiating init module 1" << std::endl;
	InitModuleTest2 initModule1("GenericRouterTestInitModule1");
	std::cout << "Instantiating init module 2" << std::endl;
	InitModuleTest2 initModule2("GenericRouterTestInitModule2");

	std::cout << "Instantiating target module 1" << std::endl;
	TargModuleTest2 targModule1("GenericRouterTestTargModule1", addresses[0]);
	std::cout << "Instantiating target module 2" << std::endl;
	TargModuleTest2 targModule2("GenericRouterTestTargModule2", addresses[1]);
	std::cout << "Instantiating target module 3" << std::endl;
	TargModuleTest2 targModule3("GenericRouterTestTargModule3", addresses[2]);
	std::cout << "Instantiating target module 4" << std::endl;
	TargModuleTest2 targModule4("GenericRouterTestTargModule4", addresses[3]);

	std::cout << "Instantiating router" << std::endl;
	::hv::communication::tlm2::utils::GenericRouter<> myRouter("MyRouter");

	std::cout << "Binding init module 1 to router" << std::endl;
	initModule1.socket.bind(myRouter.inputSocket);
	std::cout << "Binding init module 2 to router" << std::endl;
	initModule2.socket.bind(myRouter.inputSocket);

	std::cout << "Binding target module 1 to router" << std::endl;
	myRouter.outputSocket.bind(targModule1.socket);
	std::cout << "Binding target module 2 to router" << std::endl;
	myRouter.outputSocket.bind(targModule2.socket);
	std::cout << "Binding target module 3 to router" << std::endl;
	myRouter.outputSocket.bind(targModule3.socket);
	std::cout << "Binding target module 4 to router" << std::endl;
	myRouter.outputSocket.bind(targModule4.socket);

	std::cout << "Starting simulation..." << std::endl;
	::sc_core::sc_start();
}

