/**
 * @file protocol_converter_if.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Protocol Adapter
 *
 * Adapter from one protocol to another one.
 * This consists in an initiator socket of one protocol types and
 * a target socket of another type
 */

#ifndef SRC_CORE_TLM_2_PROTOCOL_ADAPTER_PROTOCOL_CONVERTER_IF_H_
#define SRC_CORE_TLM_2_PROTOCOL_ADAPTER_PROTOCOL_CONVERTER_IF_H_

namespace hv {
namespace communication {
namespace tlm2 {

// FIXME add phase conversion
template<typename TYPES_START, typename TYPES_END> struct ProtocolConverterIf {
	typedef typename TYPES_START::tlm_payload_type payload_in_type;
	typedef typename TYPES_END::tlm_payload_type payload_out_type;

	virtual ~ProtocolConverterIf() {
	}

	virtual void deepCopy(const payload_in_type& src,
			payload_out_type& dst) = 0;

	virtual void deepCopy(const payload_out_type& src,
			payload_in_type& dst) = 0;

};

}
}
}

#endif /* SRC_CORE_TLM_2_PROTOCOL_ADAPTER_PROTOCOL_CONVERTER_IF_H_ */
