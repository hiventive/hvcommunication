/**
 * @file multi_socket_callback_binder_bw.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Utils callback binder fw
 */

#ifndef HV_UTILS_TLM_2_SOCKETS_MULTI_SOCKET_CALLBACK_BINDER_BW_H_
#define HV_UTILS_TLM_2_SOCKETS_MULTI_SOCKET_CALLBACK_BINDER_BW_H_

#include <tlm>
#include <HVCommon>
#include "core/tlm-2/interfaces/fw_bw_transport_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {


template<typename TYPES> class CallbackBinderBw: public ::hv::communication::tlm2::FwBwTransportIf<
		TYPES>::bw_interface_type {
public:
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;

	// Callback type definitions
	typedef ::hv::common::CallbackImpl<
			sync_enum_type(const std::size_t &, transaction_type&, phase_type&,
					sc_core::sc_time&)> NBTransportBwCb;
	typedef ::hv::common::CallbackImpl<
			void(const std::size_t &, sc_dt::uint64, sc_dt::uint64)> InvalidateDirectMemPtrCb;

	// Constructor
	CallbackBinderBw(std::size_t id_) : mId(id_), mNbTransportCb(nullptr), mInvalidateDirectMemPtrCb(
					nullptr) {
	}

	// Set callbacks
	void setNBTransportBwCb(const NBTransportBwCb& cb) {
		mNbTransportCb = cb;
	}

	void setInvalidateDirectMemPtrCb(const InvalidateDirectMemPtrCb& cb) {
		mInvalidateDirectMemPtrCb = cb;
	}

	// BW Interface implementation
	sync_enum_type nb_transport_bw(transaction_type& txn, phase_type& p,
			sc_core::sc_time& t) {
		if (!mNbTransportCb) {
			HV_ERR("No nb_transport_bw registered")
			exit(EXIT_FAILURE);
		}
		return mNbTransportCb(mId, txn, p, t);
	}

	void invalidate_direct_mem_ptr(sc_dt::uint64 start, sc_dt::uint64 stop) {
		if (!mInvalidateDirectMemPtrCb) {
			HV_ERR("No invalidate_direct_mem_ptr registered")
			exit(EXIT_FAILURE);
		}
		mInvalidateDirectMemPtrCb(mId, start, stop);
	}

protected:
	std::size_t mId;
	NBTransportBwCb mNbTransportCb;
	InvalidateDirectMemPtrCb mInvalidateDirectMemPtrCb;

};

}
}
}
}

#endif /* HV_UTILS_TLM_2_SOCKETS_MULTI_SOCKET_CALLBACK_BINDER_BW_H_ */
