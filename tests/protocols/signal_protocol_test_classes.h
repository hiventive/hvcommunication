/**
 * @file signal_protocol_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Sockets Tests Helpers
 */

#ifndef TESTS_SIGNAL_PROTOCOL_TEST_CLASSES_H_
#define TESTS_SIGNAL_PROTOCOL_TEST_CLASSES_H_

#include "protocols/signal/signal_socket.h"

#define N_SIG_TESTS 100
bool sigIn[N_SIG_TESTS] = { true };
bool sigOut[N_SIG_TESTS] = { true };

class SignalSocketTestInitiatorModule: public ::sc_core::sc_module,
		public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::signal::SignalProtocolTypes>::bw_interface_type {
public:
	typedef ::hv::communication::tlm2::FwBwTransportIf<
			::hv::communication::tlm2::protocols::signal::SignalProtocolTypes>::bw_interface_type bw_type;
	typedef ::hv::communication::tlm2::protocols::signal::SignalProtocolTypes::tlm_payload_type payload_type;
	typedef ::hv::communication::tlm2::protocols::signal::SignalProtocolTypes::tlm_phase_type phase_type;
SignalSocketTestInitiatorModule(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket("InitiatorSocket") {
		socket((bw_type&) *this);
		SC_HAS_PROCESS(SignalSocketTestInitiatorModule);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::protocols::signal::SignalInitiatorSocket<> socket;

protected:
	void myThread() {
		// Initializing sigIn
		for (auto i = 0; i < N_SIG_TESTS; i++) {
			sigIn[i] = ::std::rand() % 2;
		}

		// Transmitting
		for (auto i = 0; i < N_SIG_TESTS; i++) {
			payload_type txn;
			::sc_core::sc_time t(::sc_core::SC_ZERO_TIME);
			txn.setValue(sigIn[i]);
			socket->b_transport(txn, t);
			if (txn.isResponseError()) {
				HV_ERR("Response Error");
				exit(EXIT_FAILURE);
			}
			::sc_core::wait((rand() % 10 + 1), ::sc_core::SC_MS);
		}

	}

	//** BW Interface **//
	tlm::tlm_sync_enum nb_transport_bw(payload_type& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}

};

class SignalSocketTestTargetModule: public ::sc_core::sc_module,
		public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::signal::SignalProtocolTypes>::fw_interface_type {
public:
	typedef ::hv::communication::tlm2::FwBwTransportIf<
			::hv::communication::tlm2::protocols::signal::SignalProtocolTypes>::fw_interface_type fw_type;
	typedef ::hv::communication::tlm2::protocols::signal::SignalProtocolTypes::tlm_payload_type payload_type;
	typedef ::hv::communication::tlm2::protocols::signal::SignalProtocolTypes::tlm_phase_type phase_type;
	SignalSocketTestTargetModule(::sc_core::sc_module_name name_) :
			::sc_core::sc_module(name_), socket("TargetSocket"), cpt(0) {
		socket((fw_type&) *this);
	}

	::hv::communication::tlm2::protocols::signal::SignalTargetSocket<> socket;

protected:

	//** FW Interface **//
	tlm::tlm_sync_enum nb_transport_fw(payload_type& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(payload_type& trans, tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(payload_type& trans) {
		return 0;
	}

	void b_transport(payload_type& trans, sc_core::sc_time& t) {
		sigOut[cpt++] = trans.getValue();
		trans.setResponseStatus(
				::hv::communication::tlm2::protocols::signal::SIGNAL_OK_RESPONSE);
	}

	int cpt;

};
#endif /* TESTS_SIGNAL_PROTOCOL_TEST_CLASSES_H_ */
