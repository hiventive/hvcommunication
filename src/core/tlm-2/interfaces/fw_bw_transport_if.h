/**
 * @file fw_bw_if.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive joint type definition for TLM forward and backward interfaces
 */

#ifndef HV_FW_BW_IF_H_
#define HV_FW_BW_IF_H_

#include <HVCommon>
#include <core/tlm-2/base_protocol/base_protocol.h>
#include <tlm>

namespace hv {
namespace communication {
namespace tlm2 {

#ifndef HV_CPLUSPLUS_NO_ALIAS_TEMPLATE

template<typename TYPES = BaseProtocolTypes> using FwTransportIf = ::tlm::tlm_fw_transport_if<TYPES>;
template<typename TYPES = BaseProtocolTypes> using BwTransportIf = ::tlm::tlm_bw_transport_if<TYPES>;

template<typename TYPES = BaseProtocolTypes>
class FwBwTransportIf: public FwTransportIf<TYPES>, public BwTransportIf<TYPES> {
public:
	typedef FwTransportIf<TYPES> fw_interface_type;
	typedef BwTransportIf<TYPES> bw_interface_type;
};

#else

template<typename TYPES = BaseProtocolTypes> struct FwTransportIf {
	typedef ::tlm::tlm_fw_transport_if<TYPES> type;
};

template<typename TYPES = BaseProtocolTypes> struct BwTransportIf {
	typedef ::tlm::tlm_bw_transport_if<TYPES> type;
};


template<typename TYPES = BaseProtocolTypes>
class FwBwTransportIf: public FwTransportIf<TYPES>::type, public BwTransportIf<TYPES>::type {
public:
	typedef typename FwTransportIf<TYPES>::type fw_interface_type;
	typedef typename BwTransportIf<TYPES>::type bw_interface_type;
};

#endif

}
}
}

#endif // HV_FW_BW_IF_H_

