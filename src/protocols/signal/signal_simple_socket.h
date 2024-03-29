/**
 * @file signal_simple_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Convenient Sockets
 *
 * Signal Sockets
 */

#ifndef HV_PROTOCOLS_SIGNAL_SIGNAL_SIMPLE_SOCKET_H_
#define HV_PROTOCOLS_SIGNAL_SIGNAL_SIMPLE_SOCKET_H_

#include <hv/common.h>

#include "../../utils/tlm-2/sockets/simple_initiator_socket.h"
#include "../../utils/tlm-2/sockets/simple_target_socket.h"
#include "signal_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace signal {

template<typename TYPES = SignalProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class SignalSimpleInitiatorSocket: public ::hv::communication::tlm2::utils::SimpleInitiatorSocket<
		32, TYPES, N, POL> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleInitiatorSocket<32, TYPES, N,
			POL> base_type;

	SignalSimpleInitiatorSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("SignalSimpleInitiatorSocket")) :
			base_type(name_) {
				BaseIO baseIOTmp;
				baseIOTmp.addIOElement(IOElement("IO_OUT", OUT_MODE));
				this->addBaseIO(baseIOTmp);
				this->addBindingRule("IO_OUT", "IO_IN");
	}
};

template<typename TYPES = SignalProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class SignalSimpleTargetSocket: public ::hv::communication::tlm2::utils::SimpleTargetSocket<
		 32, TYPES, N, POL> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleTargetSocket<32, TYPES, N, POL> base_type;

	SignalSimpleTargetSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("SignalSimpleTargetSocket")) :
			base_type(name_) {
				BaseIO baseIOTmp;
				baseIOTmp.addIOElement(IOElement("IO_IN", IN_MODE));
				this->addBaseIO(baseIOTmp);
				this->addBindingRule("IO_IN", "IO_OUT");
	}
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_SIGNAL_SIGNAL_SIMPLE_SOCKET_H_ */

