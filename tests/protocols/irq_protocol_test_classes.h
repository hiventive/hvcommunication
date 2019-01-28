/**
 * @file irq_protocol_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM IRQ Sockets Tests Helpers
 */

#ifndef TESTS_IRQ_PROTOCOL_TEST_CLASSES_H_
#define TESTS_IRQ_PROTOCOL_TEST_CLASSES_H_

#include "protocols/irq/irq_socket.h"

SC_MODULE(IRQTestModuleInit),
		public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::irq::IRQProtocolTypes>::bw_interface_type {
public:
	IRQTestModuleInit(const ::sc_core::sc_module_name &name,
			const ::hv::common::hvuint32_t& IRQID_) :
			sc_module(name), socket(), IRQID(IRQID_) {
		socket(
				(::hv::communication::tlm2::FwBwTransportIf<
						::hv::communication::tlm2::protocols::irq::IRQProtocolTypes>::bw_interface_type &) *this);
		SC_HAS_PROCESS(IRQTestModuleInit);
		SC_THREAD(myThread);
	}

	::hv::communication::tlm2::protocols::irq::IRQInitiatorSocket<> socket;
protected:
	void myThread() {
		::hv::communication::tlm2::protocols::irq::IRQPayload txn;
		txn.setID(IRQID);
		for (::hv::common::hvuint32_t i = 0; i < 10; i++) {
			std::cout << ::sc_core::sc_time_stamp() << " - " << name()
					<< ": Sending IRQ (ID: " << IRQID << ")" << std::endl;

			::sc_core::sc_time zero = ::sc_core::SC_ZERO_TIME;
			socket->b_transport(txn, zero);

			if (txn.isResponseOk()) {
				std::cout << ::sc_core::sc_time_stamp() << " - " << name()
						<< ": Target response is OK" << std::endl;
			} else {
				std::cout << ::sc_core::sc_time_stamp() << " - " << name()
						<< ": Target response is not OK" << std::endl;
				exit(EXIT_FAILURE);
			}
			::sc_core::wait(rand() % 10 + 1, ::sc_core::SC_MS);
		}
	}

	//** BW Interface **//
	tlm::tlm_sync_enum nb_transport_bw(
			::hv::communication::tlm2::protocols::irq::IRQPayload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& delay) {
		return tlm::TLM_ACCEPTED;
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
			sc_dt::uint64 end_range) {
		return;
	}

	const ::hv::common::hvuint32_t IRQID;
};

SC_MODULE(IRQTestModuleTarget),
		public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::irq::IRQProtocolTypes>::fw_interface_type {
public:
	IRQTestModuleTarget(const ::sc_core::sc_module_name &name) :
			sc_module(name), socket() {
		socket(
				(::hv::communication::tlm2::FwBwTransportIf<
						::hv::communication::tlm2::protocols::irq::IRQProtocolTypes>::fw_interface_type &) *this);
	}

	::hv::communication::tlm2::protocols::irq::IRQTargetSocket<
			::hv::communication::tlm2::protocols::irq::IRQProtocolTypes, 0,
			sc_core::SC_ONE_OR_MORE_BOUND> socket;
protected:
	//** FW Interface **//
	tlm::tlm_sync_enum nb_transport_fw(
			::hv::communication::tlm2::protocols::irq::IRQPayload& trans,
			tlm::tlm_phase& phase, sc_core::sc_time& t) {
		return tlm::TLM_ACCEPTED;
	}

	bool get_direct_mem_ptr(
			::hv::communication::tlm2::protocols::irq::IRQPayload& trans,
			tlm::tlm_dmi& dmi_data) {
		return false;
	}

	unsigned int transport_dbg(
			::hv::communication::tlm2::protocols::irq::IRQPayload& trans) {
		return 0;
	}

	void b_transport(
			::hv::communication::tlm2::protocols::irq::IRQPayload& trans,
			sc_core::sc_time& t) {
		::hv::common::hvuint32_t irqID = trans.getID();
		std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Received IRQ with ID " << irqID << std::endl;
		std::cout << "\t" << ::sc_core::sc_time_stamp() << " - " << name()
				<< ": Setting response to OK" << std::endl;
		trans.setResponseStatus(
				::hv::communication::tlm2::protocols::irq::IRQ_OK_RESPONSE);
		return;
	}
};

#endif /* TESTS_IRQ_PROTOCOL_TEST_CLASSES_H_ */
