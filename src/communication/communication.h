/*
 * @file communication.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Sep, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Communication library
 */


#ifndef HV_COMMUNICATION_COMMUNICATION_H
#define HV_COMMUNICATION_COMMUNICATION_H

#include "../core/tlm-2/helpers.h"
#include "../core/tlm-2/base_payload/base_payload.h"
#include "../core/tlm-2/base_payload/extensions.h"
#include "../core/tlm-2/base_protocol/base_protocol.h"
#include "../core/tlm-2/interfaces/fw_bw_transport_if.h"
#include "../core/tlm-2/protocol_adapter/protocol_converter_if.h"
#include "../core/tlm-2/protocol_adapter/protocol_adapter.h"

#include "../core/tlm-2/sockets/addressable_socket.h"
#include "../core/tlm-2/sockets/base_socket.h"
#include "../core/tlm-2/sockets/initiator_socket.h"
#include "../core/tlm-2/sockets/target_socket.h"
#include "../core/tlm-2/sockets/bidirectional_socket.h"

#include "../location/addressing_payload_if.h"
#include "../location/location.h"

#include "../protocols/i2c/i2c_payload.h"
#include "../protocols/i2c/i2c_protocol.h"
#include "../protocols/i2c/i2c_router.h"
#include "../protocols/i2c/i2c_socket.h"
#include "../protocols/i2c/i2c_simple_socket.h"

#include "../protocols/irq/irq_payload.h"
#include "../protocols/irq/irq_protocol.h"
#include "../protocols/irq/irq_socket.h"
#include "../protocols/irq/irq_simple_socket.h"

#include "../protocols/memory_mapped/memory_mapped_protocol_converters.h"
#include "../protocols/memory_mapped/memory_mapped_payload.h"
#include "../protocols/memory_mapped/memory_mapped_protocol.h"
#include "../protocols/memory_mapped/memory_mapped_router.h"
#include "../protocols/memory_mapped/memory_mapped_socket.h"
#include "../protocols/memory_mapped/memory_mapped_simple_socket.h"

#include "../protocols/signal/signal_payload.h"
#include "../protocols/signal/signal_protocol.h"
#include "../protocols/signal/signal_socket.h"
#include "../protocols/signal/signal_simple_socket.h"

#include "../protocols/uart/uart_payload.h"
#include "../protocols/uart/uart_protocol.h"
#include "../protocols/uart/uart_socket.h"
#include "../protocols/uart/uart_simple_socket.h"

#endif // HV_COMMUNICATION_COMMUNICATION_H

