/**
 * @file convenient_sockets_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Convenient Socket Tests Helpers
 */

#ifndef TESTS_CONVENIENT_SOCKETS_TEST_CLASSES_H_
#define TESTS_CONVENIENT_SOCKETS_TEST_CLASSES_H_

#include <tlm>
#include <HVCommon>
#include <utils/tlm-2/sockets/simple_initiator_socket.h>
#include <utils/tlm-2/sockets/simple_target_socket.h>
#include <utils/tlm-2/sockets/simple_bidirectional_socket.h>


SC_MODULE(SimpleInitSocketTestModule) {
public:
	SC_CTOR(SimpleInitSocketTestModule) {
		socket.registerNBTransportBw(this,
				&SimpleInitSocketTestModule::myNBTransportBw);
		socket.registerInvalidateDirectMemPtr(this,
				&SimpleInitSocketTestModule::myInvalidateDirectMemPtr);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::utils::SimpleInitiatorSocket<> socket;

private:
	void myThread() {
		::hv::common::hvuint8_t data[10] = { 0 };
		sc_core::sc_time t = sc_core::SC_ZERO_TIME;
		for (::hv::common::hvuint32_t i = 0; i < 10; i++) {
			::tlm::tlm_generic_payload txn;
			::hv::common::hvuint32_t length = rand() % 10 + 1;
			std::cout << sc_core::sc_time_stamp() << " - " << name()
					<< ": Sending";
			for (::hv::common::hvuint32_t j = 0; j < length; j++) {
				data[j] = static_cast<::hv::common::hvuint8_t>(rand() % 256);
				std::cout << " " << static_cast<::hv::common::hvuint32_t>(data[j]);
			}
			std::cout << std::endl;
			txn.set_data_length(length);
			txn.set_data_ptr(&(data[0]));
			socket->b_transport(txn, t);
			sc_core::wait(10.0, sc_core::SC_MS);
		}
	}

	// Dumb implementations of bw interface functions, just for callback registration test
	tlm::tlm_sync_enum myNBTransportBw(::tlm::tlm_generic_payload& txn,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		std::cout << sc_core::sc_time_stamp() << " - " << name()
				<< ": Non-blocking transport backward" << std::endl;
		return tlm::tlm_sync_enum::TLM_COMPLETED;
	}

	void myInvalidateDirectMemPtr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		std::cout << sc_core::sc_time_stamp() << " - " << name()
				<< ": Invalidate DMI" << std::endl;
	}
};

SC_MODULE(SimpleTargetSocketTestModule) {
public:
	SC_CTOR(SimpleTargetSocketTestModule) {
		socket.registerNBTransportFw(this,
				&SimpleTargetSocketTestModule::myNBTransportFw);
		socket.registerBTransport(this,
				&SimpleTargetSocketTestModule::myBTransport);
		socket.registerTransportDbg(this,
				&SimpleTargetSocketTestModule::myTransportDbg);
		socket.registerGetDirectMemPtr(this,
				&SimpleTargetSocketTestModule::myGetDirectMemPtr);
	}

	::hv::communication::tlm2::utils::SimpleTargetSocket<> socket;
private:
	// Dumb implementations of fw interface functions (except for b_transport), just for callback registration test
	tlm::tlm_sync_enum myNBTransportFw(tlm::tlm_generic_payload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< "Non-blocking transport forward" << std::endl;
		return tlm::tlm_sync_enum::TLM_COMPLETED;
	}

	unsigned int myTransportDbg(tlm::tlm_generic_payload& trans) {
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< "Debug transport" << std::endl;
		return 42;
	}

	bool myGetDirectMemPtr(tlm::tlm_generic_payload& trans,
			tlm::tlm_dmi& dmi_data) {
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< "Get direct memory pointer" << std::endl;
		return true;
	}

	void myBTransport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Blocking transport" << std::endl;
		::hv::common::hvuint32_t length = trans.get_data_length();
		::hv::common::hvuint8_t* data = trans.get_data_ptr();
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Received";
		for (::hv::common::hvuint32_t i = 0u; i < length; i++) {
			std::cout << " " << static_cast<::hv::common::hvuint32_t>(data[i]);
		}
		std::cout << std::endl;
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Acknowledging reception" << std::endl;
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}

};

// Classes for hierarchical binding tests

SC_MODULE(SimpleInitSocketHierarchicalBindingTestModule) {
public:
	SC_CTOR(SimpleInitSocketHierarchicalBindingTestModule) :
			myInsideModule(sc_core::sc_gen_unique_name("InsideInitSocket")) {
		// Hierarchical binding
		myInsideModule.socket(socket);
//		socket(myInsideModule.socket);
	}
	::hv::communication::tlm2::InitiatorSocket<> socket;
private:
	SimpleInitSocketTestModule myInsideModule;

};

SC_MODULE(SimpleTargetSocketHierarchicalBindingTestModule) {
public:
	SC_CTOR(SimpleTargetSocketHierarchicalBindingTestModule) :
			myInsideModule(sc_core::sc_gen_unique_name("InsideTargSocket")) {
		// Hierarchical binding
		socket.bind(myInsideModule.socket);
//		myInsideModule.socket(socket);
	}
	::hv::communication::tlm2::TargetSocket<> socket;
private:
	SimpleTargetSocketTestModule myInsideModule;
};

//** Simple bidirectional socket test classes **//
SC_MODULE(SimpleBidirectionalSocketTestModule) {
public:
	SC_CTOR(SimpleBidirectionalSocketTestModule) {
		socket.registerBTransport(this, &SimpleBidirectionalSocketTestModule::myBTransport);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::utils::SimpleBidirectionalSocket<> socket;
protected:
	void myThread() {
		::hv::common::hvuint8_t data[10] = { 0 };
		sc_core::sc_time t = sc_core::SC_ZERO_TIME;
		for (::hv::common::hvuint32_t i = 0; i < 10; i++) {
			::tlm::tlm_generic_payload txn;
			::hv::common::hvuint32_t length = rand() % 10 + 1;
			std::cout << sc_core::sc_time_stamp() << " - " << name()
					<< ": Sending";
			for (::hv::common::hvuint32_t j = 0; j < length; j++) {
				data[j] = static_cast<::hv::common::hvuint8_t>(rand() % 256);
				std::cout << " " << static_cast<::hv::common::hvuint32_t>(data[j]);
			}
			std::cout << std::endl;
			txn.set_data_length(length);
			txn.set_data_ptr(&(data[0]));
			socket->b_transport(txn, t);
			sc_core::wait(rand() % 10 + 1, sc_core::SC_MS);
		}
	}

	void myBTransport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Blocking transport" << std::endl;
		::hv::common::hvuint32_t length = trans.get_data_length();
		::hv::common::hvuint8_t* data = trans.get_data_ptr();
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Received";
		for (::hv::common::hvuint32_t i = 0u; i < length; i++) {
			std::cout << " " << static_cast<::hv::common::hvuint32_t>(data[i]);
		}
		std::cout << std::endl;
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Acknowledging reception" << std::endl;
		trans.set_response_status(tlm::TLM_OK_RESPONSE);
	}
};

SC_MODULE(SimpleBidirectionalSocketHierarchicalBindingTestModule) {
public:
	SC_CTOR(SimpleBidirectionalSocketHierarchicalBindingTestModule) :
			myInsideModule(sc_core::sc_gen_unique_name("InsideBidirSocket")) {
		// Hierarchical binding
		myInsideModule.socket.bindHierarchical(socket);
	}
	::hv::communication::tlm2::BidirectionalSocket<> socket;
private:
	SimpleBidirectionalSocketTestModule myInsideModule;
};

#endif /* TESTS_CONVENIENT_SOCKETS_TEST_CLASSES_H_ */
