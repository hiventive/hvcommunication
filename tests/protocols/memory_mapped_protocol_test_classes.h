/**
 * @file memory_mapped_protocol_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Modules for Hiventive TLM Memory Mapped Protocol, Socket, Router tests
 */

#ifndef TESTS_MEMORY_MAPPED_PROTOCOL_TEST_CLASSES_H_
#define TESTS_MEMORY_MAPPED_PROTOCOL_TEST_CLASSES_H_

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include <hv/common.h>
#include <gtest/gtest.h>

#include "protocols/memory_mapped/memory_mapped_router.h"
#include "protocols/memory_mapped/memory_mapped_socket.h"
#include "protocols/memory_mapped/memory_mapped_simple_socket.h"

// We have 5 modules for tests with the following mmAddresses:
::hv::common::hvaddr_t mmAddresses[5] = { 0, 10, 20, 30, 40 };
// The 5 modules have the following mmSizes:
::std::size_t mmSizes[5] = { 3, 5, 7, 9, 11 };

// This is what one module looks like:
class MemoryMappedModule: public ::sc_core::sc_module,
		public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes> {
public:
	typedef ::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes protocol_type;
	typedef protocol_type::tlm_payload_type payload_type;
	typedef protocol_type::tlm_phase_type phase_type;
	typedef ::hv::communication::tlm2::FwBwTransportIf<protocol_type> base_type;

	MemoryMappedModule(::sc_core::sc_module_name name_,
			const ::hv::common::hvaddr_t &addr_, const ::std::size_t &size_) :
			::sc_core::sc_module(name_), socket("Socket", addr_, size_), myStartAddress(
					addr_), mySize(size_) {

		// Binding interface
		socket.bind((base_type&) *this);

//		// Adding registers
//		for (::hv::common::hvaddr_t i = 0u; i < size_; i += 4u) {
//			mainRF.createRegister(i, 32u, "Reg" + std::to_string(i),
//					"This is a register", ::hv::common::RW);
//			// Generating random 32-bit value
//			mainRF.getRegister(i) = std::rand();
//		}
		SC_HAS_PROCESS(MemoryMappedModule);
		SC_THREAD(myThread);

//#ifndef NDEBUG
//		std::cout << "DEBUG: Listing parameters" << std::endl;
//		auto debugBroker(::cci::cci_get_broker());
//		for (auto pDebug : ::cci::cci_get_broker().get_param_handles()) {
//			std::cout << pDebug.name() << " = " << pDebug.get_cci_value()
//					<< std::endl;
//		}
//#endif
	}

	virtual ~MemoryMappedModule() {
	}

	::hv::communication::tlm2::protocols::memorymapped::MemoryMappedBidirectionalSocket<> socket;

protected:
	void myThread() {
		for (int iteration = 0; iteration < 10; iteration++) {
			// We send a b_transport to one of the other 4 modules
			payload_type txn;
			::sc_core::sc_time t(::sc_core::SC_ZERO_TIME);

			// Picking a destination address
			::hv::common::hvaddr_t destination = myStartAddress;
			int destIndex;
			do {
				destIndex = std::rand() % 5;
				destination = mmAddresses[destIndex] * 4u;
			} while (destination == myStartAddress);
			destination += (std::rand() % mmSizes[destIndex]) * 4u;

			// Setting destination address
			txn.setAddress(destination);

			// Do we request a read or a write?
			bool requestRead = std::rand() % 2;
			if (requestRead) {
				txn.setCommand(
						::hv::communication::tlm2::protocols::memorymapped::MemoryMappedCommand::MEM_MAP_READ_COMMAND);
				std::cout << this->name()
						<< ": Requesting reading value of size 4 from absolute address "
						<< destination << std::endl;
				const ::std::size_t dataLength = 4;
				txn.setDataLength(dataLength);
				socket->b_transport(txn, t);

				// Checking if it's ok
				if (txn.isResponseOk()) {
					std::cout << this->name()
							<< ": Got OK response requesting reading from "
							<< destination << std::endl;
					// Reading value from payload
					::hv::common::hvuint8_t* data = txn.getDataPtr();
					HV_ASSERT(dataLength == txn.getDataLength(), "Length error");
					int message(data[0]);
					message |= (int(data[1]) << 8);
					message |= (int(data[2]) << 16);
					message |= (int(data[3]) << 24);
					std::cout << this->name() << ": I read " << message
							<< " from " << destination << std::endl;
					delete[] data;
				} else {
					exit(EXIT_FAILURE);
				}

			} else {
				const ::std::size_t dataLength = 4;
				::hv::common::hvuint8_t* data =
						new ::hv::common::hvuint8_t[dataLength];
				txn.setCommand(
						::hv::communication::tlm2::protocols::memorymapped::MemoryMappedCommand::MEM_MAP_WRITE_COMMAND);
				// Creating message
				int message = std::rand();
				data[0] = static_cast<::hv::common::hvuint8_t>(message & 0xFF);
				data[1] = static_cast<::hv::common::hvuint8_t>((message >> 8)
						& 0xFF);
				data[2] = static_cast<::hv::common::hvuint8_t>((message >> 16)
						& 0xFF);
				data[3] = static_cast<::hv::common::hvuint8_t>((message >> 24)
						& 0xFF);

				// Setting message to payload
				txn.setDataPtr(data);
				txn.setDataLength(dataLength);

				// Sending message
				std::cout << this->name() << ": Writing value " << message
						<< " to address " << destination << std::endl;
				socket->b_transport(txn, t);

				// Checking
				if (txn.isResponseOk()) {
					std::cout << this->name() << ": Got OK response writing to "
							<< destination << std::endl;
					delete[] data;
				} else {
					delete[] data;
					exit(EXIT_FAILURE);
				}
			}
			// Waiting for 1 to 10 ms before next request
			::sc_core::wait(1 + (std::rand() % 9), ::sc_core::SC_MS);
		}

	}

	// BW interface
	tlm::tlm_sync_enum nb_transport_bw(payload_type& trans, phase_type& phase,
			sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}

	// FW interface
	tlm::tlm_sync_enum nb_transport_fw(payload_type& trans, phase_type& phase,
			sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(payload_type& trans, tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(payload_type& trans) {
		return 0;
	}

	void b_transport(payload_type& trans, sc_core::sc_time& t) {
		::hv::common::hvaddr_t destination = trans.getAddress();
		if (trans.getCommand()
				== ::hv::communication::tlm2::protocols::memorymapped::MemoryMappedCommand::MEM_MAP_READ_COMMAND) {
			// Asked for read command
			std::cout << "\t" << this->name()
					<< ": Received read request of size "
					<< trans.getDataLength() << " at relative address "
					<< destination << std::endl;
			HV_ASSERT(trans.getDataLength() == 4u, "Length error");
//			std::cout << "\t" << this->name() << ": Value is "
//					<< ::hv::common::hvuint32_t(mainRF(destination))
//					<< std::endl;
			::hv::common::hvuint8_t* data =
					new ::hv::common::hvuint8_t[trans.getDataLength()];
			// Reading
//			mainRF(destination).read(data, 4u);
			trans.setDataPtr(data);
			// Setting ok response
			trans.setResponseStatus(
					::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_OK_RESPONSE);
		} else {
			// Asked for write command
			std::cout << "\t" << this->name()
					<< ": Received write request of size "
					<< trans.getDataLength() << " at relative address "
					<< destination << std::endl;
			HV_ASSERT(trans.getDataLength() == 4u, "Length error");
			// Writing data
			std::cout << "\t" << this->name() << ": Writing data..."
					<< std::endl;
//			mainRF(destination).write(trans.getDataPtr(),
//					trans.getDataLength());
//			std::cout << "\t" << this->name() << ": New value is "
//					<< ::hv::common::hvuint32_t(mainRF(destination))
//					<< std::endl;
			// Setting ok response
			trans.setResponseStatus(
					::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_OK_RESPONSE);
		}
	}

// These 2 members are redundant with Location parameter of MemoryMappedSocket socket -> only for easier testing
	::hv::common::hvaddr_t myStartAddress;
	::std::size_t mySize;
};

// Classes for ::tlm::tlm_base_protocol compatibility

template<int N> class CompTestMemMapTargetModule: public ::sc_core::sc_module {
public:
	typedef ::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes::tlm_payload_type payload_type;

	CompTestMemMapTargetModule(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket("Socket") {
		socket.registerBTransport(this,
				&CompTestMemMapTargetModule::mBTransport);
	}
	~CompTestMemMapTargetModule() {
	}

	::hv::communication::tlm2::protocols::memorymapped::MemoryMappedSimpleTargetSocket<32,
			::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes,
			N> socket;
protected:
	void mBTransport(payload_type& txn, ::sc_core::sc_time& t) {
		std::cout << this->name() << ": Received " << txn.getCommandStr()
				<< " command of length " << txn.getDataLength() << " @"
				<< txn.getAddress() << std::endl;
		std::cout << this->name() << ": Allowing DMI" << std::endl;
		txn.setDMIAllowed(true);
		std::cout << this->name() << ": Setting OK response" << std::endl;
		txn.setResponseStatus(
				::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_OK_RESPONSE);
	}
};

class CompTestGenericTargetModule: public ::sc_core::sc_module {
public:
	typedef ::tlm::tlm_base_protocol_types::tlm_payload_type payload_type;

	CompTestGenericTargetModule(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket("Socket") {
		socket.register_b_transport(this,
				&CompTestGenericTargetModule::mBTransport);
	}

	~CompTestGenericTargetModule() {
	}

	::tlm_utils::simple_target_socket<CompTestGenericTargetModule> socket;
protected:
	void mBTransport(payload_type& txn, ::sc_core::sc_time& t) {
		std::cout << this->name() << ": Received " << txn.get_command()
				<< " command of length " << txn.get_data_length() << " @"
				<< txn.get_address() << std::endl;
		std::cout << this->name() << "Allowing DMI" << std::endl;
		txn.set_dmi_allowed(true);
		std::cout << this->name() << "Setting OK response" << std::endl;
		txn.set_response_status(::tlm::TLM_OK_RESPONSE);
	}
};

template<int N> class CompTestMemMapInitModule: public ::sc_core::sc_module {
public:
	CompTestMemMapInitModule(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket("Socket") {
		SC_HAS_PROCESS(CompTestMemMapInitModule);
		SC_THREAD(mThread);
	}

	~CompTestMemMapInitModule() {
	}

	::hv::communication::tlm2::protocols::memorymapped::MemoryMappedSimpleInitiatorSocket<32, ::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes,
		N> socket;
protected:
	void mThread() {
		typedef ::hv::communication::tlm2::protocols::memorymapped::MemoryMappedProtocolTypes::tlm_payload_type payload_type;
		payload_type txn;
		::sc_core::sc_time zeroTime(::sc_core::SC_ZERO_TIME);
		::hv::common::hvaddr_t addr(std::rand() % 1024);
		std::size_t len(1 + (std::rand() % 16));
		txn.setAddress(addr);
		txn.setCommand(
				::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_READ_COMMAND);
		txn.setDataLength(len);
		std::cout << this->name() << ": Sending read command of length " << len
				<< " @" << addr << std::endl;
		socket->b_transport(txn, zeroTime);
		ASSERT_TRUE(txn.isResponseOk())<< "Response should be OK";
		ASSERT_EQ(txn.getAddress(), addr)<< "Address shouldn't have changed";
		ASSERT_EQ(txn.getDataLength(), len)<< "Length shouldn't have changed";
		ASSERT_EQ(txn.getCommand(), ::hv::communication::tlm2::protocols::memorymapped::MEM_MAP_READ_COMMAND)<< "Command shoudln't have changed";
		ASSERT_TRUE(txn.isDMIAllowed())<< "DMI should've been allowed";
	}
};

class CompTestGenericInitModule: public ::sc_core::sc_module {
public:
	CompTestGenericInitModule(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket("Socket") {
		SC_HAS_PROCESS(CompTestGenericInitModule);
		SC_THREAD(mThread);
	}

	~CompTestGenericInitModule() {
	}

	::tlm_utils::simple_initiator_socket<CompTestGenericInitModule> socket;

protected:
	void mThread() {
		::sc_core::wait(5, ::sc_core::SC_MS);
		typedef ::tlm::tlm_base_protocol_types::tlm_payload_type payload_type;
		payload_type txn;
		::sc_core::sc_time zeroTime(::sc_core::SC_ZERO_TIME);
		::hv::common::hvaddr_t addr(std::rand() % 1024);
		std::size_t len(1 + (std::rand() % 16));
		txn.set_address(addr);
		txn.set_command(::tlm::TLM_READ_COMMAND);
		txn.set_data_length(len);
		std::cout << this->name() << ": Sending read command of length " << len
				<< " @" << addr << std::endl;
		socket->b_transport(txn, zeroTime);
		ASSERT_TRUE(txn.is_response_ok())<< "Response should be OK";
		ASSERT_EQ(txn.get_address(), addr)<< "Address shouldn't have changed";
		ASSERT_EQ(txn.get_data_length(), len)<< "Length shouldn't have changed";
		ASSERT_EQ(txn.get_command(), ::tlm::TLM_READ_COMMAND)<< "Command shoudln't have changed";
		ASSERT_TRUE(txn.is_dmi_allowed())<< "DMI should've been allowed";
	}
};

#endif /* TESTS_MEMORY_MAPPED_PROTOCOL_TEST_CLASSES_H_ */
