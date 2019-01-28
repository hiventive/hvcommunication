/**
 * @file signal_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Signal Sockets
 *
 * Signal Sockets
 */

#ifndef HV_PROTOCOLS_SIGNAL_SIGNAL_SOCKET_H_
#define HV_PROTOCOLS_SIGNAL_SIGNAL_SOCKET_H_

#include <HVCommon>
#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/sockets/target_socket.h"
#include "signal_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace signal {

template<typename TYPES = SignalProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ONE_OR_MORE_BOUND> class SignalInitiatorSocket: public InitiatorSocket<
		 32, TYPES, N, POL> {
public:
	typedef InitiatorSocket<32, TYPES, N, POL> base_type;

	SignalInitiatorSocket(::sc_core::sc_module_name name_ = ::sc_core::sc_gen_unique_name("SignalInitiatorSocket")) :
			base_type(name_) {
				BaseIO baseIOTmp;
				baseIOTmp.addIOElement(IOElement("IO_OUT", OUT_MODE));
				this->addBaseIO(baseIOTmp);
				this->addBindingRule("IO_OUT", "IO_IN");
	}
};

template<typename TYPES = SignalProtocolTypes, int N = 1,
		::sc_core::sc_port_policy POL = ::sc_core::SC_ZERO_OR_MORE_BOUND> class SignalTargetSocket: public TargetSocket<
		 32, TYPES, N, POL> {
public:
	typedef TargetSocket<32, TYPES, N, POL> base_type;

	SignalTargetSocket(::sc_core::sc_module_name name_ = ::sc_core::sc_gen_unique_name("SignalTargetSocket")) :
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

#endif /* HV_PROTOCOLS_SIGNAL_SIGNAL_SOCKET_H_ */

