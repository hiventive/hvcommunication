/**
 * @file payload_address_handler.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Oct, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Payload Address Handler
 *
 * These meta-classes allow GenericRouter to work with any
 * payload type, as long as the corresponding GenericRouterAddressGetter
 * is implemented.
 * By default, payloads are supposed to be implementing AddressingPayloadIf.
 * If they are, there is nothing to do.
 * This file also implements GenericRouterAddressGetter for ::tlm::tlm_base_protocol_types.
 *
 * To turn GenericRouter compatible with your own custom payload not implementing
 * AddressingPayloadIf, just follow the example of 3. which implements it for
 * ::tlm::tlm_base_protocol_types.
 */

#ifndef SRC_UTILS_TLM_2_ROUTING_PAYLOAD_ADDRESS_HANDLER_H_
#define SRC_UTILS_TLM_2_ROUTING_PAYLOAD_ADDRESS_HANDLER_H_

#include <tlm>

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

/**
 * 1. Generic router address getter interface
 */
template<typename TYPES, typename LOCATION_TYPE> class GenericRouterAddressGetterIf {
public:
	typedef typename TYPES::tlm_payload_type payload_type;
	typedef typename LOCATION_TYPE::address_type address_type;

	virtual ~GenericRouterAddressGetterIf() {
	}

	virtual address_type getAddress(payload_type& txn) const = 0;
	virtual void setAddress(payload_type& txn, const address_type& addr) = 0;
};

/**
 * 2. Generic router address getter generic implementation (compatible with any
 * payload implementing AddressingPayloadIf)
 */

template<typename TYPES, typename LOCATION_TYPE> class PayloadAddressHandler: public GenericRouterAddressGetterIf<
		TYPES, LOCATION_TYPE> {
public:
	typedef GenericRouterAddressGetterIf<TYPES, LOCATION_TYPE> base_type;

	virtual ~PayloadAddressHandler() {
	}

	virtual typename base_type::address_type getAddress(
			typename base_type::payload_type& txn) const {
		return txn.getAddress();
	}

	virtual void setAddress(typename base_type::payload_type& txn,
			const typename base_type::address_type& addr) {
		txn.setAddress(addr);
	}
};

/**
 * 3. Generic router address getter implementation for ::tlm::tlm_generic_payload
 */
template<typename LOCATION_TYPE> class PayloadAddressHandler<
		::tlm::tlm_base_protocol_types, LOCATION_TYPE> : public GenericRouterAddressGetterIf<
		::tlm::tlm_base_protocol_types, LOCATION_TYPE> {
public:
	typedef GenericRouterAddressGetterIf<::tlm::tlm_base_protocol_types,
			LOCATION_TYPE> base_type;
	virtual ~PayloadAddressHandler() {
	}

	virtual typename base_type::address_type getAddress(
			typename base_type::payload_type& txn) const {
		return txn.get_address();
	}

	virtual void setAddress(typename base_type::payload_type& txn,
			const typename base_type::address_type& addr) {
		txn.set_address(addr);
	}

};

}
}
}
}




#endif /* SRC_UTILS_TLM_2_ROUTING_PAYLOAD_ADDRESS_HANDLER_H_ */
