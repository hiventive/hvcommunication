/*
 * addressing_payload.h
 *
 *  Created on: Sep 7, 2018
 *      Author: bbarrois
 */

#ifndef SRC_LOCATION_ADDRESSING_PAYLOAD_IF_H_
#define SRC_LOCATION_ADDRESSING_PAYLOAD_IF_H_

namespace hv {
namespace communication {
namespace tlm2 {

template<typename ADDR_TYPE> class AddressingPayloadIf {
public:
	virtual ~AddressingPayloadIf() {
	}

	// Methods to be implemented
	virtual ADDR_TYPE getAddress() const = 0;
	virtual void setAddress(const ADDR_TYPE& addr_) = 0;
};

}
}
}

#endif /* SRC_LOCATION_ADDRESSING_PAYLOAD_IF_H_ */
