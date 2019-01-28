/**
 * @file multi_socket_test_classes.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Modules for Hiventive TLM Utils Multi Socket Test
 */
#ifndef TESTS_MULTI_SOCKET_TEST_CLASSES_H_
#define TESTS_MULTI_SOCKET_TEST_CLASSES_H_

#include "utils/tlm-2/sockets/multi_initiator_socket.h"
#include "utils/tlm-2/sockets/multi_target_socket.h"


SC_MODULE(MultiInitModule) {
public:
	SC_CTOR(MultiInitModule) :
			socket() {
		socket.registerNBTransportBw(this, &MultiInitModule::myNBTransportBW);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::utils::MultiInitiatorSocket<> socket;

protected:
	void myThread() {
		for (std::size_t i = 0; i < socket.getNSockets(); i++) {
			::tlm::tlm_generic_payload trans;
			sc_core::sc_time t(sc_core::SC_ZERO_TIME);
			std::cout << name() << ": Sending b_transport to target socket "
					<< i << std::endl;
			socket[i]->b_transport(trans, t);
		}
	}

	::tlm::tlm_sync_enum myNBTransportBW(const std::size_t& i,
			::tlm::tlm_generic_payload& trans, ::tlm::tlm_phase &phase,
			::sc_core::sc_time& t) {
		std::cout << name() << ": Received nb_transport_bw from target socket "
				<< i << std::endl;
		return ::tlm::tlm_sync_enum::TLM_COMPLETED;
	}
};

SC_MODULE(TargetModule), public ::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type {
public:
	SC_CTOR(TargetModule) :
			socket() {
		socket.bind((::hv::communication::tlm2::FwBwTransportIf<>::fw_interface_type &) *this);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::TargetSocket<> socket;
protected:
	void myThread() {
		::tlm::tlm_generic_payload trans;
		::tlm::tlm_phase phase(::tlm::tlm_phase_enum::UNINITIALIZED_PHASE);
		::sc_core::sc_time t(sc_core::SC_ZERO_TIME);
		socket->nb_transport_bw(trans, phase, t);
	}

	// FW Interface Implementation
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
		std::cout << "\t" << name() << ": Received b_transport" << std::endl;
	}
};

SC_MODULE(MultiInitEncapsulatingModule) {
public:
	SC_CTOR(MultiInitEncapsulatingModule) :
			socket(), innerMultiInitModule("myInnerModule") {
//		socket.bind((MultiInitiatorSocket<>&) innerMultiInitModule.socket);
		innerMultiInitModule.socket.bind(socket);
	}

	::hv::communication::tlm2::utils::MultiInitiatorSocket<> socket;
protected:
	MultiInitModule innerMultiInitModule;
};

SC_MODULE(InitModule), public ::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type {
public:
	SC_CTOR(InitModule) :
			socket() {
		socket.bind((::hv::communication::tlm2::FwBwTransportIf<>::bw_interface_type&) *this);
		SC_THREAD(myThread);
	}
	::hv::communication::tlm2::InitiatorSocket<> socket;
protected:

	void myThread() {
		::tlm::tlm_generic_payload trans;
		sc_core::sc_time t(sc_core::SC_ZERO_TIME);
		std::cout << name() << ": Sending b_transport to target socket "
				<< std::endl;
		socket->b_transport(trans, t);
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

SC_MODULE(MultiTargetModule) {
public:
	SC_CTOR(MultiTargetModule) :
			socket() {
		socket.registerBTransport(this, &MultiTargetModule::myBTransport);
	}

	::hv::communication::tlm2::utils::MultiTargetSocket<> socket;
protected:
	void myBTransport(const std::size_t& i, ::tlm::tlm_generic_payload& trans,
			::sc_core::sc_time& t) {
		std::cout << "\t" << name() << ": Received b_transport from " << i
				<< std::endl;
	}
};

SC_MODULE(MultiTargetEncapsulatingModule) {
public:
	SC_CTOR(MultiTargetEncapsulatingModule) :
			innerMultiTargetModule("myInnerModule"), socket() {
		socket.bind(innerMultiTargetModule.socket);
	}

	MultiTargetModule innerMultiTargetModule;
	::hv::communication::tlm2::utils::MultiTargetSocket<> socket;
};

#endif /* TESTS_MULTI_SOCKET_TEST_CLASSES_H_ */
