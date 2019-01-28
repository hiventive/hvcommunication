/**
 * @file memory_mapped_protocol_test.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Memory Mapped Test
 */

#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <tlm>

#include "memory_mapped_protocol_test_classes.h"

using namespace ::hv::communication::tlm2::protocols::memorymapped;

class MemoryMappedTest: public ::testing::Test {
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {

	}
};

TEST_F(MemoryMappedTest, GeneralTest) {
	HV_SYSTEMC_RESET_CONTEXT

	// Instantiating memory-mapped modules
	std::cout << "### Instantiating Modules ###" << std::endl;
	std::cout << "Instantiating Memory-Mapped module 0... ";
	MemoryMappedModule module0("Module0", mmAddresses[0] * 4u, mmSizes[0] * 4u);
	std::cout << "Done" << std::endl;
	std::cout << "Instantiating Memory-Mapped module 1... ";
	MemoryMappedModule module1("Module1", mmAddresses[1] * 4u, mmSizes[1] * 4u);
	std::cout << "Done" << std::endl;
	std::cout << "Instantiating Memory-Mapped module 2... ";
	MemoryMappedModule module2("Module2", mmAddresses[2] * 4u, mmSizes[2] * 4u);
	std::cout << "Done" << std::endl;
	std::cout << "Instantiating Memory-Mapped module 3... ";
	MemoryMappedModule module3("Module3", mmAddresses[3] * 4u, mmSizes[3] * 4u);
	std::cout << "Done" << std::endl;
	std::cout << "Instantiating Memory-Mapped module 4... ";
	MemoryMappedModule module4("Module4", mmAddresses[4] * 4u, mmSizes[4] * 4u);
	std::cout << "Done" << std::endl;

	// Instantiating routers
	std::cout << "Instantiating Memory-Mapped router... ";
	MemoryMappedRouter<> router("MemoryMappedRouter");
	std::cout << "Done" << std::endl;

	// Binding bidirectional socket of module to router
	std::cout << "### Binding module's sockets to router's ###" << std::endl;

	std::cout << "Binding Module0's initiator to router... ";
	module0.socket.initiator().bind(router.inputSocket);
	std::cout << "Done" << std::endl;
	std::cout << "Binding router's output to Module0's target... ";
	router.outputSocket.bind(module0.socket.target());
	std::cout << "Done" << std::endl;

	std::cout << "Binding Module1's initiator to router... ";
	module1.socket.initiator().bind(router.inputSocket);
	std::cout << "Done" << std::endl;
	std::cout << "Binding router's output to Module1's target... ";
	router.outputSocket.bind(module1.socket.target());
	std::cout << "Done" << std::endl;

	std::cout << "Binding Module2's initiator to router... ";
	module2.socket.initiator().bind(router.inputSocket);
	std::cout << "Done" << std::endl;
	std::cout << "Binding router's output to Module2's target... ";
	router.outputSocket.bind(module2.socket.target());
	std::cout << "Done" << std::endl;

	std::cout << "Binding Module3's initiator to router... ";
	module3.socket.initiator().bind(router.inputSocket);
	std::cout << "Done" << std::endl;
	std::cout << "Binding router's output to Module3's target... ";
	router.outputSocket.bind(module3.socket.target());
	std::cout << "Done" << std::endl;

	std::cout << "Binding Module4's initiator to router... ";
	module4.socket.initiator().bind(router.inputSocket);
	std::cout << "Done" << std::endl;
	std::cout << "Binding router's output to Module4's target... ";
	router.outputSocket.bind(module4.socket.target());
	std::cout << "Done" << std::endl;

	// Starting simulation
	::sc_core::sc_start();
}

TEST_F(MemoryMappedTest, CompatibilityTestUnlimitedBind1) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "### Instantiating Modules ###" << std::endl;
	std::cout
			<< "Instantiating Hiventive Memory-Mapped module with target socket...";
	CompTestMemMapTargetModule<0> hvTargModule("HVMemMapTargetModule");
	std::cout << "Done" << std::endl;

	std::cout
			<< "Instantiating Hiventive Memory-Mapped module with initiator socket...";
	CompTestMemMapInitModule<1> hvInitModule("HVMemMapInitiatorModule");
	std::cout << "Done" << std::endl;

	std::cout
			<< "Instantiating Hiventive Memory-Mapped module with generic socket...";
	CompTestGenericInitModule genericInitModule("GenericInitiatorModule");
	std::cout << "Done" << std::endl;

	std::cout << "Binding HV Init Module to HV Targ Module...";
	hvInitModule.socket.bind(hvTargModule.socket);
	std::cout << "Done" << std::endl;

	std::cout << "Binding Generic Init Module to HV Targ Module...";
	genericInitModule.socket.bind(hvTargModule.socket);
	std::cout << "Done" << std::endl;

	std::cout << "Starting simulation..." << std::endl;
	// Starting simulation
	::sc_core::sc_start();
}

TEST_F(MemoryMappedTest, CompatibilityTestUnlimitedBind2) {
	HV_SYSTEMC_RESET_CONTEXT
	std::cout << "### Instantiating Modules ###" << std::endl;

	std::cout
			<< "Instantiating Hiventive Memory-Mapped module with initiator socket...";
	CompTestMemMapInitModule<0> hvInitModule("HVMemMapInitModule2");
	std::cout << "Done" << std::endl;

	std::cout << "Instantiating generic module with target socket...";
	CompTestGenericTargetModule genericTargModule("GenericTargModule2");
	std::cout << "Done" << std::endl;

	std::cout << "Binding...";
	hvInitModule.socket.bind(genericTargModule.socket);
	std::cout << "Done" << std::endl;

	std::cout << "Starting simulation..." << std::endl;
	// Starting simulation
	::sc_core::sc_start();
}

TEST_F(MemoryMappedTest, GenericPayloadTest) {
	HV_SYSTEMC_RESET_CONTEXT

	typedef MemoryMappedProtocolTypes::tlm_payload_type MemoryMappedPayload;
	typedef ::tlm::tlm_generic_payload GenericPayload;

	GenericPayload genericPayload;
	MemoryMappedPayload memoryMappedPayload;
	TLMGenericMemoryMapProtocolConverter converter;

	unsigned char data = 'H';

	// Generic Payload -> Memory Mapped Payload
	genericPayload.set_command(tlm::TLM_WRITE_COMMAND);
	genericPayload.set_address(0xFF);
	genericPayload.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
	genericPayload.set_data_length(4);
	genericPayload.set_streaming_width(4);
	genericPayload.set_byte_enable_ptr(0);
	genericPayload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

	converter.deepCopy(genericPayload, memoryMappedPayload);

	ASSERT_EQ(memoryMappedPayload.getAddress(), genericPayload.get_address());
	ASSERT_EQ(memoryMappedPayload.getCommand(), MEM_MAP_WRITE_COMMAND);
	ASSERT_EQ(memoryMappedPayload.getDataPtr(), genericPayload.get_data_ptr());
	ASSERT_EQ(memoryMappedPayload.getDataLength(), genericPayload.get_data_length());
	// ASSERT_EQ(memoryMappedPayload.getStreamingWidth(), genericPayload.get_streaming_width()); // TODO
	// ASSERT_EQ(memoryMappedPayload.getByteEnablePtr(), genericPayload.get_byte_enable_ptr()); // TODO
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_INCOMPLETE_RESPONSE);

	// Memory Mapped Payload -> Generic Payload
	memoryMappedPayload.setCommand(MEM_MAP_READ_COMMAND);
	memoryMappedPayload.setAddress(0xDE);
	memoryMappedPayload.setDataPtr(reinterpret_cast<unsigned char*>(&data));
	memoryMappedPayload.setDataLength(8);
	// payload.setStreamingWidth(8); // TODO
	// payload.setByteEnablePtr(1); // TODO
	memoryMappedPayload.setResponseStatus(
			::hv::communication::tlm2::protocols::memorymapped::MemoryMappedResponseStatus::MEM_MAP_INCOMPLETE_RESPONSE);

	converter.deepCopy(memoryMappedPayload, genericPayload);

	ASSERT_EQ(genericPayload.get_address(), memoryMappedPayload.getAddress());
	ASSERT_EQ(genericPayload.get_command(), tlm::TLM_READ_COMMAND);
	ASSERT_EQ(genericPayload.get_data_ptr(), memoryMappedPayload.getDataPtr());
	ASSERT_EQ(genericPayload.get_data_length(), memoryMappedPayload.getDataLength());
	// ASSERT_EQ(genericPayload.get_streaming_width(), memoryMappedPayload.getStreamingWidth()); // TODO
	// ASSERT_EQ(genericPayload.get_byte_enable_ptr(), memoryMappedPayload.getByteEnablePtr()); // TODO
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_INCOMPLETE_RESPONSE);

	// Check Responses
	genericPayload.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_ADDRESS_ERROR_RESPONSE);

	genericPayload.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_BURST_ERROR_RESPONSE);

	genericPayload.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_BYTE_ENABLE_ERROR_RESPONSE);

	genericPayload.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_COMMAND_ERROR_RESPONSE);

	genericPayload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_GENERIC_ERROR_RESPONSE);

	genericPayload.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_INCOMPLETE_RESPONSE);

	genericPayload.set_response_status(tlm::TLM_OK_RESPONSE);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getResponseStatus(), MEM_MAP_OK_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_ADDRESS_ERROR_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_ADDRESS_ERROR_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_BURST_ERROR_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_BURST_ERROR_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_BYTE_ENABLE_ERROR_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_COMMAND_ERROR_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_COMMAND_ERROR_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_GENERIC_ERROR_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_GENERIC_ERROR_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_INCOMPLETE_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_INCOMPLETE_RESPONSE);

	memoryMappedPayload.setResponseStatus(MEM_MAP_OK_RESPONSE);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_response_status(), tlm::TLM_OK_RESPONSE);

	// Check Commands
	genericPayload.set_command(tlm::TLM_WRITE_COMMAND);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getCommand(), MEM_MAP_WRITE_COMMAND);

	genericPayload.set_command(tlm::TLM_READ_COMMAND);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getCommand(), MEM_MAP_READ_COMMAND);

	genericPayload.set_command(tlm::TLM_IGNORE_COMMAND);
	converter.deepCopy(genericPayload, memoryMappedPayload);
	ASSERT_EQ(memoryMappedPayload.getCommand(), MEM_MAP_IGNORE_COMMAND);

	memoryMappedPayload.setCommand(MEM_MAP_WRITE_COMMAND);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_command(), tlm::TLM_WRITE_COMMAND);

	memoryMappedPayload.setCommand(MEM_MAP_READ_COMMAND);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_command(), tlm::TLM_READ_COMMAND);

	memoryMappedPayload.setCommand(MEM_MAP_IGNORE_COMMAND);
	converter.deepCopy(memoryMappedPayload, genericPayload);
	ASSERT_EQ(genericPayload.get_command(), tlm::TLM_IGNORE_COMMAND);
}
