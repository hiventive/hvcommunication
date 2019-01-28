/**
 * @file i2c_protocol_test.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM I2C protocol tests
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>

#include "i2c_protocol_test_classes.h"

using namespace ::hv::communication::tlm2::protocols::i2c;

class I2CTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(I2CTest, RoutingTest) {
	HV_SYSTEMC_RESET_CONTEXT
	// Instantiating i2c modules
	std::cout << "### Instantiating Modules ###" << std::endl;
	std::cout << "Instantiating i2c Master 0... ";
	I2CMasterModule moduleMaster0("I2CMaster0", i2cMastersAddresses[0]);
	std::cout << "Done" << std::endl;
	std::cout << "Instantiating i2c Master 1... ";
	I2CMasterModule moduleMaster1("I2CMaster1", i2cMastersAddresses[1]);
	std::cout << "Done" << std::endl;

	std::cout << "Instantiating i2c Slave 0... ";
	I2CSlaveModule moduleSlave0("I2CSlave0", i2cSlavesAddresses[0]);
	std::cout << "Done" << std::endl;
	std::cout << "Instantiating i2c Slave 1... ";
	I2CSlaveModule moduleSlave1("I2CSlave1", i2cSlavesAddresses[1]);
	std::cout << "Done" << std::endl;

	std::cout << "Instantiating i2c router... ";
	I2CRouter I2CRouter("I2CRouter");
	std::cout << "Done" << std::endl;

	std::cout << "### Binding I2C module's sockets to router's ###"
			<< std::endl;
	std::cout << "Binding i2c Master 0 to router... ";
	moduleMaster0.socket.bind(I2CRouter.inputSocket);
	std::cout << "Done" << std::endl;
	std::cout << "Binding i2c Master 1 to router... ";
	moduleMaster1.socket.bind(I2CRouter.inputSocket);
	std::cout << "Done" << std::endl;

	std::cout << "Binding router to i2c Slave 0... ";
	I2CRouter.outputSocket.bind(moduleSlave0.socket);
	std::cout << "Done" << std::endl;

	std::cout << "Binding router to i2c Slave 1... ";
	I2CRouter.outputSocket.bind(moduleSlave1.socket);
	std::cout << "Done" << std::endl;

	std::cout << "Simulation... ";
	::sc_core::sc_start();
	std::cout << "Done" << std::endl;

}
