/**
 * @file bidirectional_socket_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Bidirectional Socket Tests Helpers
 */

#ifndef TESTS_BIDIRECTIONAL_SOCKET_TEST_CLASSES_H_
#define TESTS_BIDIRECTIONAL_SOCKET_TEST_CLASSES_H_

#include <systemc>
#include <tlm>
#include <HVCommon>
#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/sockets/target_socket.h"
#include "core/tlm-2/sockets/bidirectional_socket.h"


class TestModule: public ::sc_core::sc_module, public ::hv::communication::tlm2::FwBwTransportIf<> {
	SC_HAS_PROCESS(TestModule);
public:
	TestModule(const ::sc_core::sc_module_name &name_) :
			::sc_core::sc_module(name_), socket() {
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Constructor" << std::endl;
		// Binding socket
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Binding interfaces" << std::endl;
		socket((FwBwTransportIf<>&) *this);

		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::BidirectionalSocket<> socket;

protected:
	void myThread() {
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Entering thread" << std::endl;
		for (::hv::common::hvuint32_t i = 0u; i < 10u; i++) {
			::tlm::tlm_generic_payload trans;
			::sc_core::sc_time t(sc_core::sc_time(10, ::sc_core::SC_MS));
			std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
					<< ") - Sending b_transport" << std::endl;
			socket->b_transport(trans, t);
			::sc_core::wait(100, ::sc_core::SC_MS);
		}
	}

	// BW interface
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& tran,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}

	// FW interface
	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
			tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
		return 0;
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
		return;
	}

};

class TestModuleBidir: public ::sc_core::sc_module, public ::hv::communication::tlm2::FwBwTransportIf<> {
public:
	TestModuleBidir(const ::sc_core::sc_module_name &name_) :
		::sc_core::sc_module(name_), socket(), myInt(0) {
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Constructor" << std::endl;
		// Binding socket
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Binding interfaces" << std::endl;
		socket((FwBwTransportIf<>&) *this);

	}

	::hv::communication::tlm2::BidirectionalSocket<32, ::hv::communication::tlm2::BaseProtocolTypes> socket;
	int myInt;

protected:
	void myFunction() {
		::tlm::tlm_generic_payload trans;
		::sc_core::sc_time t(sc_core::sc_time(10, ::sc_core::SC_MS));

		int x = myInt;
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Sending b_transport - transmitting value " << x
				<< std::endl;
		trans.set_data_ptr(reinterpret_cast<unsigned char*>(&x));
		socket->b_transport(trans, t);
	}

	// BW interface
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& tran,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	// FW interface
	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
			tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
		return 0;
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Received b_transport - " << "Received value "
				<< *reinterpret_cast<int*>(trans.get_data_ptr()) << std::endl;
		myInt = *reinterpret_cast<int*>(trans.get_data_ptr());
		this->myFunction();
		return;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}
};

class TestModuleInit: public ::sc_core::sc_module,
public ::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type {
	SC_HAS_PROCESS(TestModuleInit);
public:
	TestModuleInit(const ::sc_core::sc_module_name &name_) :
			sc_module(name_), socket() {
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Constructor" << std::endl;
		// Binding socket
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Binding interfaces" << std::endl;
		socket((::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type&) *this);

		SC_THREAD(myThread);
	}
	::hv::communication::tlm2::InitiatorSocket<> socket;

protected:
	void myThread() {
		for (::hv::common::hvuint32_t i = 0u; i < 10; i++) {
			::tlm::tlm_generic_payload trans;
			::sc_core::sc_time t(sc_core::sc_time(10, ::sc_core::SC_MS));

			int data = rand() % 100;
			std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
					<< ") - Generated random value " << data << std::endl;
			std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
					<< ") - Sending b_transport - transmitting value " << data
					<< std::endl;
			trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
			socket->b_transport(trans, t);
			::sc_core::wait(1, ::sc_core::SC_MS);
		}
	}

	// BW interface
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& tran,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}
};

class TestModuleTarget: public ::sc_core::sc_module,
public ::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type {
public:
	TestModuleTarget(const ::sc_core::sc_module_name &name_) :
		::sc_core::sc_module(name_), socket() {
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Constructor" << std::endl;
		// Binding socket
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Binding interfaces" << std::endl;
		socket((::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type&) *this);

	}

	::hv::communication::tlm2::TargetSocket<> socket;

protected:

	// FW interface
	tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans,
			tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
		return 0;
	}

	void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
		int x;
		unsigned char* xChar = trans.get_data_ptr();
		std::memcpy(&x, xChar, 4);
		std::cout << sc_core::sc_time_stamp() << ": TestModule(" << name()
				<< ") - Received b_transport - " << "Received value " << x
				<< std::endl;

		return;
	}

};

#endif /* TESTS_BIDIRECTIONAL_SOCKET_TEST_CLASSES_H_ */
