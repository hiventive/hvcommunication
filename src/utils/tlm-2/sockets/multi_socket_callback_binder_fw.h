/**
 * @file multi_socket_callback_binder_fw.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Utils callback binder fw
 */

#ifndef HV_UTILS_TLM_2_SOCKETS_MULTI_SOCKET_CALLBACK_BINDER_FW_H_
#define HV_UTILS_TLM_2_SOCKETS_MULTI_SOCKET_CALLBACK_BINDER_FW_H_

#include <tlm>
#include <HVCommon>
#include "core/tlm-2/interfaces/fw_bw_transport_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

template<typename TYPES> class CallbackBinderFw: public ::hv::communication::tlm2::FwBwTransportIf<
		TYPES>::fw_interface_type {
public:
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;

	// Callback type definitions
	typedef ::hv::common::CallbackImpl<
			sync_enum_type(const std::size_t &, transaction_type&, phase_type&,
					sc_core::sc_time&)> NBTransportFwCb;
	typedef ::hv::common::CallbackImpl<
			void(const std::size_t &, transaction_type&, sc_core::sc_time&)> BTransportCb;
	typedef ::hv::common::CallbackImpl<
			unsigned int(const std::size_t &, transaction_type&)> TransportDbgCb;
	typedef ::hv::common::CallbackImpl<
			bool(const std::size_t &, transaction_type&, tlm::tlm_dmi&)> GetDirectMemPtrCb;

	// Constructor
	CallbackBinderFw(std::size_t id_) :
			mId(id_), mNBTransportFwCb(nullptr), mBTransportCb(nullptr), mTransportDbgCb(
					nullptr), mGetDirectMemPtrCb(nullptr) {
	}

	// Set callbacks
	void setNBTransportFwCb(const NBTransportFwCb& cb) {
		mNBTransportFwCb = cb;
	}

	void setBTransportCb(const BTransportCb& cb) {
		mBTransportCb = cb;
	}

	void setTransportDbgCb(const TransportDbgCb& cb) {
		mTransportDbgCb = cb;
	}

	void setGetDirectMemPtrCb(const GetDirectMemPtrCb& cb) {
		mGetDirectMemPtrCb = cb;
	}

	// FW Interface implementation
	sync_enum_type nb_transport_fw(transaction_type& txn, phase_type& p,
			sc_core::sc_time& t) {
		if (!mNBTransportFwCb) {
			HV_ERR("No nb_transport_fw registered")
			exit(EXIT_FAILURE);
		}
		return mNBTransportFwCb(mId, txn, p, t);
	}

	void b_transport(transaction_type& trans, sc_core::sc_time& t) {
		if (!mBTransportCb) {
			HV_ERR("No b_transport registered")
			exit(EXIT_FAILURE);
		}
		mBTransportCb(mId, trans, t);
	}

	bool get_direct_mem_ptr(transaction_type& trans, tlm::tlm_dmi& dmi_data) {
		if (!mGetDirectMemPtrCb) {
			HV_ERR("No get_direct_mem_ptr registered")
			exit(EXIT_FAILURE);
		}
		return mGetDirectMemPtrCb(mId, trans, dmi_data);
	}

	unsigned int transport_dbg(transaction_type& trans) {
		if (!mTransportDbgCb) {
			HV_ERR("No transport_dbg registered")
			exit(EXIT_FAILURE);
		}
		return mTransportDbgCb(mId, trans);
	}
protected:
	std::size_t mId;
	NBTransportFwCb mNBTransportFwCb;
	BTransportCb mBTransportCb;
	TransportDbgCb mTransportDbgCb;
	GetDirectMemPtrCb mGetDirectMemPtrCb;
};

}
}
}
}

#endif /* HV_UTILS_TLM_2_SOCKETS_MULTI_SOCKET_CALLBACK_BINDER_FW_H_ */
