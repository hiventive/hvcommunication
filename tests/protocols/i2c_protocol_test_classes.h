/**
 * @file i2c_protocol_test_classes.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Modules for Hiventive TLM I2C protocol (including router)
 */

#ifndef TESTS_I2C_PROTOCOL_TEST_CLASSES_H_
#define TESTS_I2C_PROTOCOL_TEST_CLASSES_H_

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <hv/common.h>

#include "protocols/i2c/i2c_socket.h"
#include "protocols/i2c/i2c_router.h"

typedef ::hv::communication::tlm2::protocols::i2c::i2c_address_type i2c_address_type;

i2c_address_type i2cMastersAddresses[2] = { 0x0, 0x10 };
i2c_address_type i2cSlavesAddresses[2] = { 0x1, 0x11 };

class I2CMasterModule: public ::sc_core::sc_module,
public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::i2c::I2CProtocolTypes>::bw_interface_type {
public:
	typedef ::hv::communication::tlm2::protocols::i2c::I2CProtocolTypes protocol_type;
	typedef protocol_type::tlm_payload_type payload_type;
	typedef protocol_type::tlm_phase_type phase_type;
	typedef ::hv::communication::tlm2::FwBwTransportIf<protocol_type>::bw_interface_type base_type;

	I2CMasterModule(::sc_core::sc_module_name name_,
			const i2c_address_type& addr_) :
			::sc_core::sc_module(name_), socket("Socket", addr_), myAddress(
					addr_), mData(new ::hv::common::hvuint8_t[10]) {

		// Binding interface
		socket.bind((base_type&) *this);
		SC_HAS_PROCESS(I2CMasterModule);
		SC_THREAD(myThread);


	}

	~I2CMasterModule() {
		delete[] mData;
	}

	::hv::communication::tlm2::protocols::i2c::I2CInitiatorSocket<> socket;

protected:
	void myThread() {
		for (int iteration = 0; iteration < 10; iteration++) {
			payload_type txn;
			::sc_core::sc_time t(::sc_core::SC_ZERO_TIME);

			// Picking a destination address
			i2c_address_type destination = i2cSlavesAddresses[rand() % 2];
			txn.setAddress(destination);

			// Requesting read or write?
			bool requestRead = std::rand() % 2;
			if (requestRead) {
				txn.setCommand(
						::hv::communication::tlm2::protocols::i2c::I2C_READ_COMMAND);
				std::cout << this->name() << ": Requesting I2C read @ address "
						<< destination << std::endl;
				socket->b_transport(txn, t);
				std::size_t len = txn.getDataLength();
				HV_ASSERT(txn.isResponseOk(), "Reading was refused");
				if (!len) {
					std::cout << this->name() << "No data to read" << std::endl;
				} else {
					::hv::common::hvuint8_t* data = txn.getDataPtr();
					std::cout << this->name() << "I read";
					for (auto cpt = 0u; cpt < len; cpt++) {
						std::cout << " " << static_cast<unsigned int>(data[cpt]);
					}
					std::cout << std::endl;
				}

			} else {
				// Request write
				txn.setCommand(
						::hv::communication::tlm2::protocols::i2c::I2C_WRITE_COMMAND);
				std::cout << this->name() << ": Requesting I2C write @ address "
						<< destination << std::endl;
				std::size_t len = rand() % 10 + 1;
				txn.setDataLength(len);
				std::cout << this->name() << "I am writing";
				for (auto i = 0u; i < len; i++) {
					mData[i] = static_cast<::hv::common::hvuint8_t>(rand());
					std::cout << " " << static_cast<unsigned int>(mData[i]);
				}
				txn.setDataPtr(mData);
				std::cout << std::endl;
				socket->b_transport(txn, t);
				HV_ASSERT(txn.isResponseOk(), "Writing was refused");
			}
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

	i2c_address_type myAddress;
	::hv::common::hvuint8_t* mData;

};

class I2CSlaveModule: public ::sc_core::sc_module,
		public ::hv::communication::tlm2::FwBwTransportIf<
				::hv::communication::tlm2::protocols::i2c::I2CProtocolTypes>::fw_interface_type {
public:
	typedef ::hv::communication::tlm2::protocols::i2c::I2CProtocolTypes protocol_type;
	typedef protocol_type::tlm_payload_type payload_type;
	typedef protocol_type::tlm_phase_type phase_type;
	typedef ::hv::communication::tlm2::FwBwTransportIf<protocol_type>::fw_interface_type base_type;

	I2CSlaveModule(::sc_core::sc_module_name name_,
			const i2c_address_type& addr_) :
			::sc_core::sc_module(name_), socket("Socket", addr_), myAddress(
					addr_), mData(new ::hv::common::hvuint8_t[10]) {
		// Binding interface
		socket.bind((base_type&) *this);
	}

	~I2CSlaveModule() {
		delete[] mData;
	}

	::hv::communication::tlm2::protocols::i2c::I2CTargetSocket<> socket;
protected:
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
		if (trans.getCommand()
				== ::hv::communication::tlm2::protocols::i2c::I2C_READ_COMMAND) {
			// Read cmd
			std::cout << "\t" << this->name() << "I received a read command"
					<< std::endl;
			// Generating data to read
			std::size_t len = rand() % 10 + 1;
			trans.setDataLength(len);
			std::cout << "\t" << this->name() << "I am sending read data";
			for (auto i = 0u; i < len; i++) {
				mData[i] = static_cast<::hv::common::hvuint8_t>(rand());
				std::cout << " " << static_cast<unsigned int>(mData[i]);
			}
			std::cout << std::endl;
			trans.setDataPtr(mData);
			trans.setResponseStatus(
					::hv::communication::tlm2::protocols::i2c::I2C_OK_RESPONSE);
		} else {
			// Write cmd
			std::cout << "\t" << this->name() << "I received a write command"
					<< std::endl;
			::hv::common::hvuint8_t* data = trans.getDataPtr();
			std::size_t len = trans.getDataLength();
			std::cout << this->name() << "I am written ";
			for (auto cpt = 0u; cpt < len; cpt++) {
				std::cout << " " << static_cast<unsigned int>(data[cpt]);
			}
			std::cout << std::endl;
			trans.setResponseStatus(
					::hv::communication::tlm2::protocols::i2c::I2C_OK_RESPONSE);

		}
	}

	i2c_address_type myAddress;
	::hv::common::hvuint8_t* mData;
};

#endif /* TESTS_I2C_PROTOCOL_TEST_CLASSES_H_ */
