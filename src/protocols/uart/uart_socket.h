/**
 * @file uart_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Socket
 *
 * UART Socket
 */

#ifndef HV_CORE_TLM_2_PROTOCOLS_UART_UART_SOCKET_H_
#define HV_CORE_TLM_2_PROTOCOLS_UART_UART_SOCKET_H_

#include <tlm>
#include <HVCommon>
#include <HVConfiguration>
#include "core/tlm-2/sockets/bidirectional_socket.h"
#include "protocols/uart/uart_payload.h"
#include "protocols/uart/uart_protocol.h"

#define UART_DEFAULT_VALID_BITS 8
#define UART_DEFAULT_STOP_BITS 1
#define UART_DEFAULT_ENABLE_BITS ::hv::communication::tlm2::protocols::uart::UartEnableFlags::UART_BAUD_RATE_ENABLED
#define UART_DEFAULT_BAUD_RATE 100000

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace uart {

enum UartEnableFlags {
	UART_PARITY_ENABLED = 0x1,
	UART_VALID_BITS_ENABLED = 0x2,
	UART_STOP_BITS_ENABLED = 0x4,
	UART_BAUD_RATE_ENABLED = 0x8
};

template<typename TYPES = UartProtocolTypes, int N_INIT = 1, int N_TARG = 1,
		sc_core::sc_port_policy POL_INIT = sc_core::SC_ONE_OR_MORE_BOUND,
		sc_core::sc_port_policy POL_TARG = sc_core::SC_ONE_OR_MORE_BOUND> class UartSocket: public BidirectionalSocket<32,
		TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG> {
public:
	typedef BidirectionalSocket<32, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG> base_type;

	UartSocket(const ::hv::common::hvuint8_t &validBits_ =
			UART_DEFAULT_VALID_BITS, const ::hv::common::hvuint8_t &stopBits_ =
			UART_DEFAULT_STOP_BITS, const ::hv::common::hvuint8_t &enableBits_ =
			UART_DEFAULT_ENABLE_BITS, const ::hv::common::hvuint32_t &baudRate_ = UART_DEFAULT_BAUD_RATE) :
	base_type(), mValidBits(std::string(this->name()) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mValidBits"),
	validBits_), mStopBits(std::string(this->name()) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mStopBits"),
	stopBits_), mEnableBits(std::string(this->name()) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mEnableBits"),
	enableBits_), mBaudRate(std::string(this->name()) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mBaudRate"), baudRate_) {
		// Adding base IO
		BaseIO baseIOTmp;
		baseIOTmp.addIOElement("RX", IN_MODE);
		this->target().addBaseIO(baseIOTmp);
		this->target().addBindingRule("RX", "TX");
		BaseIO baseIOTmp2;
		baseIOTmp2.addIOElement("TX", OUT_MODE);
		this->initiator().addBaseIO(baseIOTmp2);
		this->initiator().addBindingRule("TX", "RX");
		
		this->registerAllCallbacks();
	}

	UartSocket(::sc_core::sc_module_name name_, const ::hv::common::hvuint8_t &validBits_ = UART_DEFAULT_VALID_BITS,
	const ::hv::common::hvuint8_t &stopBits_ = UART_DEFAULT_STOP_BITS,
	const ::hv::common::hvuint8_t &enableBits_ = UART_DEFAULT_ENABLE_BITS, const ::hv::common::hvuint32_t &baudRate_ = UART_DEFAULT_BAUD_RATE) :
	base_type(name_), mValidBits(std::string(name_) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mValidBits"),
	validBits_), mStopBits(std::string(name_) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mStopBits"),
	stopBits_), mEnableBits(std::string(name_) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mEnableBits"),
	enableBits_), mBaudRate(std::string(name_) + HV_SYSTEMC_HIERARCHY_CHAR + std::string("mBaudRate"), baudRate_) {
		// Adding base IO
		BaseIO baseIOTmp;
		baseIOTmp.addIOElement("RX", IN_MODE);
		this->target().addBaseIO(baseIOTmp);
		this->target().addBindingRule("RX", "TX");
		BaseIO baseIOTmp2;
		baseIOTmp2.addIOElement("TX", OUT_MODE);
		this->initiator().addBaseIO(baseIOTmp2);
		this->initiator().addBindingRule("TX", "RX");

		this->registerAllCallbacks();
	}

	virtual ~UartSocket() {

	}

private:
	void postWriteCbValidBitsChange(
	const cci::cci_param_write_event<::hv::common::hvuint8_t> &ev) {
		// TODO
		HV_WARN("This method is unimplemented yet")
	}

	void postWriteCbStopBitsChange(
	const cci::cci_param_write_event<::hv::common::hvuint8_t> &ev) {
		// TODO
		HV_WARN("This method is unimplemented yet")
	}

	void postWriteCbEnableBitsChange(
	const cci::cci_param_write_event<::hv::common::hvuint8_t> &ev) {
		// TODO
		HV_WARN("This method is unimplemented yet")
	}

	void postWriteCbBaudRateChange(
	const cci::cci_param_write_event<::hv::common::hvuint32_t> &ev) {
		// TODO
		HV_WARN("This method is unimplemented yet")
	}

	void registerAllCallbacks() {
//		// Registering callback for mValidBits modification
//		cci_callback_untyped_handle cciValidBitsHandle =
//		mValidBits.register_post_write_callback(std::bind(
//		&UartSocketBase::postWriteCbValidBitsChange, this, std::placeholders::_1));
//		// Binding callback with initiator
//		mStopBits.register_post_write_callback(std::bind(
//		&UartSocketBase::postWriteCbStopBitsChange, this, std::placeholders::_1));
//		mEnableBits.register_post_write_callback(std::bind(
//		&UartSocketBase::postWriteCbEnableBitsChange, this, std::placeholders::_1));
//		mBaudRate.register_post_write_callback(std::bind(
//		&UartSocketBase::postWriteCbBaudRateChange, this, std::placeholders::_1));
	}

protected:
	::hv::cfg::Param<::hv::common::hvuint8_t> mValidBits;
	::hv::cfg::Param<::hv::common::hvuint8_t> mStopBits;
	::hv::cfg::Param<::hv::common::hvuint8_t> mEnableBits;
	::hv::cfg::Param<::hv::common::hvuint32_t> mBaudRate;
};


}
}
}
}
}

#endif /* HV_CORE_TLM_2_PROTOCOLS_UART_UART_SOCKET_H_ */
