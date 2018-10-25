/**
 * @file uart_simple_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM UART Convenient Sockets
 *
 * Signal Sockets
 */

#ifndef HV_PROTOCOLS_UART_UART_SIMPLE_SOCKET_H_
#define HV_PROTOCOLS_UART_UART_SIMPLE_SOCKET_H_

#include <hv/common.h>

#include "../../utils/tlm-2/sockets/simple_bidirectional_socket.h"
#include "uart_protocol.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace protocols {
namespace uart {

template<typename TYPES = UartProtocolTypes, int N_INIT = 1, int N_TARG = 1,
		::sc_core::sc_port_policy POL_INIT = ::sc_core::SC_ONE_OR_MORE_BOUND,
		::sc_core::sc_port_policy POL_TARG = ::sc_core::SC_ONE_OR_MORE_BOUND> class UARTSimpleBidirectionalSocket: public ::hv::communication::tlm2::utils::SimpleBidirectionalSocket<
		 32, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG> {
public:
	typedef ::hv::communication::tlm2::utils::SimpleBidirectionalSocket<32, TYPES,
			N_INIT, N_TARG, POL_INIT, POL_TARG> base_type;

	UARTSimpleBidirectionalSocket(::sc_core::sc_module_name name_ =
			::sc_core::sc_gen_unique_name("UARTSimpleInitiatorSocket")) :
			base_type(name_) {
			// Adding base IO
			BaseIO baseIOTmp;
			baseIOTmp.addIOElement("RX", IN_MODE);
			this->target().addBaseIO(baseIOTmp);
			this->target().addBindingRule("RX", "TX");
			BaseIO baseIOTmp2;
			baseIOTmp2.addIOElement("TX", OUT_MODE);
			this->initiator().addBaseIO(baseIOTmp2);
			this->initiator().addBindingRule("TX", "RX");
		}
};

}
}
}
}
}

#endif /* HV_PROTOCOLS_SIGNAL_UART_UART_SOCKET_H_ */

