/**
 * @file router_base_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Base Router Tests Helpers
 */

#ifndef TESTS_GENERIC_ROUTER_TEST_CLASSES_H_
#define TESTS_GENERIC_ROUTER_TEST_CLASSES_H_

#include <systemc>
#include <tlm>
#include "utils/tlm-2/routing/generic_router.h"
#include "location/location.h"
#include "core/tlm-2/sockets/addressable_socket.h"
#include "core/tlm-2/interfaces/fw_bw_transport_if.h"

//** Second test - memory mapped **//
::hv::common::hvaddr_t addresses[4] = { 0x0000, 0x0010, 0x0100, 0x1000 };
std::size_t blockSize = 10u;

//** Addressable target socket **//
class TargSocketTest: public ::hv::communication::tlm2::TargetSocket<>,
		public ::hv::communication::tlm2::AddressableSocket<> {
public:
	TargSocketTest(const std::string& name_,
			const ::hv::common::hvaddr_t &startAddress_,
			const std::size_t &size_) :
			::hv::communication::tlm2::TargetSocket<>(name_.c_str()), ::hv::communication::tlm2::AddressableSocket<>(
					this->name(), startAddress_, size_) {
		std::cout << name_ << ": (start_address, size) == "
				<< ::hv::communication::tlm2::Location<>(startAddress_, size_)
				<< std::endl;
	}

	virtual ~TargSocketTest() {
	}
};

SC_MODULE(InitModuleTest),
		public ::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type {
public:
	InitModuleTest(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket() {
		socket.bind(
				(::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type&) *this);
		SC_HAS_PROCESS(InitModuleTest);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::InitiatorSocket<> socket;
protected:
	void myThread() {
		::tlm::tlm_generic_payload trans;
		::sc_core::sc_time t(::sc_core::sc_time(10, ::sc_core::SC_MS));
		for (::hv::common::hvuint32_t i = 0u; i < 10u; i++) {
			::hv::common::hvaddr_t destAddr(addresses[rand() % 4]);
			destAddr += rand() % blockSize;
			trans.set_address(destAddr);
			std::cout << sc_core::sc_time_stamp() << ": " << name()
					<< " - Sending b_transport to address 0x" << std::hex
					<< destAddr << std::dec << std::endl;
			socket->b_transport(trans, t);
			::sc_core::wait((rand() % 10) + 1, ::sc_core::SC_MS);
		}
	}

	// BW interface
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}
};

SC_MODULE(TargModuleTest),
		public ::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type {
public:
	TargModuleTest(::sc_core::sc_module_name name_,
			const ::hv::common::hvaddr_t &address_, const std::size_t &size_) :
			::sc_core::sc_module(name_), socket("Socket", address_, size_) {
		socket.bind(
				(::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type&) *this);
	}

	TargSocketTest socket;
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
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Received blocking transport at address 0x" << std::hex
				<< trans.get_address() << std::dec << std::endl;
	}
};

//** Second test - non memory mapped **//
::hv::common::hvaddr_t addresses2[4] = { 0x0000, 0x0010, 0x0100, 0x1000 };

//** Addressable target socket **//
class TargSocketTest2: public ::hv::communication::tlm2::TargetSocket<>,
		public ::hv::communication::tlm2::AddressableSocket<> {
public:
	TargSocketTest2(const std::string& name_,
			const ::hv::common::hvaddr_t &address_) :
			::hv::communication::tlm2::TargetSocket<>(name_.c_str()), ::hv::communication::tlm2::AddressableSocket<>(
					this->name(), address_) {
		std::cout << name_ << ": address == " << address_ << std::endl;
	}

	virtual ~TargSocketTest2() {
	}
};

SC_MODULE(InitModuleTest2),
		public ::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type {
public:
	InitModuleTest2(::sc_core::sc_module_name name_) :
			sc_module(name_), socket() {
		socket.bind(
				(::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type&) *this);
		SC_HAS_PROCESS(InitModuleTest2);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::InitiatorSocket<> socket;
protected:
	void myThread() {
		::tlm::tlm_generic_payload trans;
		::sc_core::sc_time t(sc_core::sc_time(10, ::sc_core::SC_MS));
		for (::hv::common::hvuint32_t i = 0u; i < 10u; i++) {
			::hv::common::hvaddr_t destAddr(addresses[rand() % 4]);
			trans.set_address(destAddr);
			std::cout << sc_core::sc_time_stamp() << ": " << name()
					<< " - Sending b_transport to address 0x" << std::hex
					<< destAddr << std::dec << std::endl;
			socket->b_transport(trans, t);
			::sc_core::wait((rand() % 10) + 1, ::sc_core::SC_MS);
		}
	}

	// BW interface
	tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}
};

SC_MODULE(TargModuleTest2),
		public ::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type {
public:
	TargModuleTest2(::sc_core::sc_module_name name_,
			const ::hv::common::hvaddr_t &address_) :
			sc_module(name_), socket("Socket", address_) {
		socket.bind(
				(::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type&) *this);

	}

	TargSocketTest2 socket;
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
		std::cout << "\t" << sc_core::sc_time_stamp() << " - " << name()
				<< ": Received blocking transport at address 0x" << std::hex
				<< trans.get_address() << std::dec << std::endl;
	}
};

#endif /* TESTS_GENERIC_ROUTER_TEST_CLASSES_H_ */
