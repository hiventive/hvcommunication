/*
 * @file HVCommunication.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Communication library
 */


#ifndef HV_COMMUNICATION_H
#define HV_COMMUNICATION_H

#include "hv/communication/core/tlm-2/helpers.h"
#include "hv/communication/core/tlm-2/base_payload/base_payload.h"
#include "hv/communication/core/tlm-2/base_payload/extensions.h"
#include "hv/communication/core/tlm-2/base_protocol/base_protocol.h"
#include "hv/communication/core/tlm-2/interfaces/fw_bw_transport_if.h"
#include "hv/communication/core/tlm-2/protocol_adapter/protocol_converter_if.h"
#include "hv/communication/core/tlm-2/protocol_adapter/protocol_adapter.h"

#include "hv/communication/core/tlm-2/sockets/addressable_socket.h"
#include "hv/communication/core/tlm-2/sockets/initiator_socket.h"
#include "hv/communication/core/tlm-2/sockets/target_socket.h"
#include "hv/communication/core/tlm-2/sockets/bidirectional_socket.h"

#include "hv/communication/location/addressing_payload_if.h"
#include "hv/communication/location/location.h"

#include "hv/communication/protocols/i2c/i2c_payload.h"
#include "hv/communication/protocols/i2c/i2c_protocol.h"
#include "hv/communication/protocols/i2c/i2c_router.h"
#include "hv/communication/protocols/i2c/i2c_socket.h"
#include "hv/communication/protocols/i2c/i2c_simple_socket.h"

#include "hv/communication/protocols/irq/irq_payload.h"
#include "hv/communication/protocols/irq/irq_protocol.h"
#include "hv/communication/protocols/irq/irq_socket.h"
#include "hv/communication/protocols/irq/irq_simple_socket.h"

#include "hv/communication/protocols/memory_mapped/memory_mapped_protocol_converters.h"
#include "hv/communication/protocols/memory_mapped/memory_mapped_payload.h"
#include "hv/communication/protocols/memory_mapped/memory_mapped_protocol.h"
#include "hv/communication/protocols/memory_mapped/memory_mapped_router.h"
#include "hv/communication/protocols/memory_mapped/memory_mapped_socket.h"
#include "hv/communication/protocols/memory_mapped/memory_mapped_simple_socket.h"

#include "hv/communication/protocols/signal/signal_payload.h"
#include "hv/communication/protocols/signal/signal_protocol.h"
#include "hv/communication/protocols/signal/signal_socket.h"
#include "hv/communication/protocols/signal/signal_simple_socket.h"

#include "hv/communication/protocols/uart/uart_payload.h"
#include "hv/communication/protocols/uart/uart_protocol.h"
#include "hv/communication/protocols/uart/uart_socket.h"
#include "hv/communication/protocols/uart/uart_simple_socket.h"

#endif // HV_COMMUNICATION_H

