/**
 * @file multi_target_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Utils Multi Target Socket
 */

#ifndef HV_UTILS_TLM_2_SOCKETS_MULTI_TARGET_SOCKET_H_
#define HV_UTILS_TLM_2_SOCKETS_MULTI_TARGET_SOCKET_H_

#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/sockets/target_socket.h"
#include "multi_socket_callback_binder_fw.h"


namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

// MultiInitiatorSocket forward declaration
template<unsigned int BUSWIDTH, typename TYPES, int N, sc_core::sc_port_policy POL> class MultiInitiatorSocket;

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N = 0,
		sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class MultiTargetSocket: public TargetSocket<
		BUSWIDTH, TYPES, N, POL> {
public:
	typedef MultiTargetSocket<BUSWIDTH, TYPES, N, POL> this_type;
	typedef TargetSocket<BUSWIDTH, TYPES, N, POL> base_type;
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;
	typedef typename CallbackBinderFw<TYPES>::BTransportCb BTransportCb_type;
	typedef typename CallbackBinderFw<TYPES>::NBTransportFwCb NBTransportFwCb_type;
	typedef typename CallbackBinderFw<TYPES>::TransportDbgCb TransportDbgCb_type;
	typedef typename CallbackBinderFw<TYPES>::GetDirectMemPtrCb GetDirectMemPtrCb_type;

	using base_type::bind;

	MultiTargetSocket(
			const std::string &name_ = sc_core::sc_gen_unique_name(
					"MultiTargetSocket")) :
			base_type(name_.c_str()), mBTransportCb(nullptr), mNBTransportFwCb(
					nullptr), mTransportDbgCb(nullptr), mGetDirectMemPtrCb(
					nullptr), mHierarchicalBind(nullptr), mDummy(2713u), mBoundHierarchically(
					false) {

	}

	virtual ~MultiTargetSocket() {
		for (auto it = mBinders.begin(); it != mBinders.end(); ++it) {
			delete *it;
		}
	}
public:
	//** Callbacks registration **//
	void registerBTransport(const BTransportCb_type &cb) {
		if (mBTransportCb) {
			HV_WARN(
					"Callback already registered for b_transport. Ignoring new registration")
			return;
		}
		mBTransportCb = cb;
	}

	template<typename MODULE> void registerBTransport(MODULE* mod,
			void (MODULE::*cb)(const std::size_t &, transaction_type&,
					sc_core::sc_time&)) {
		BTransportCb_type cbTmp(cb, mod);
		registerBTransport(cbTmp);
	}

	void registerNBTransportFw(const NBTransportFwCb_type &cb) {
		if (mNBTransportFwCb) {
			HV_WARN(
					"Callback already registered for nb_transport_fw. Ignoring new registration")
			return;
		}
		mNBTransportFwCb = cb;
	}

	template<typename MODULE> void registerNBTransportFw(MODULE* mod,
			sync_enum_type (MODULE::*cb)(const std::size_t &, transaction_type&,
					phase_type&, sc_core::sc_time&)) {
		NBTransportFwCb_type cbTmp(cb, mod);
		registerNBTransportFw(cbTmp);
	}

	void registerTransportDbg(const TransportDbgCb_type &cb) {
		if (mTransportDbgCb) {
			HV_WARN(
					"Callback already registered for transport_dbg. Ignoring new registration")
			return;
		}
		mTransportDbgCb = cb;
	}

	template<typename MODULE> void registerTransportDbg(MODULE* mod,
			unsigned int (MODULE::*cb)(const std::size_t &,
					transaction_type&)) {
		TransportDbgCb_type cbTmp(cb, mod);
		registerTransportDbg(cbTmp);
	}

	void registerGetDirectMemPtr(const GetDirectMemPtrCb_type &cb) {
		if (mGetDirectMemPtrCb) {
			HV_WARN(
					"Callback already registered for get_direct_mem_ptr. Ignoring new registration")
			return;
		}
		mGetDirectMemPtrCb = cb;
	}

	template<typename MODULE> void registerGetDirectMemPtr(MODULE* mod,
			bool (MODULE::*cb)(const std::size_t &, transaction_type&,
					tlm::tlm_dmi&)) {
		GetDirectMemPtrCb_type cbTmp(cb, mod);
		registerGetDirectMemPtr(cbTmp);
	}

	//** Interface and export access **//
	virtual typename FwBwTransportIf<TYPES>::fw_interface_type& get_base_interface() {
		mBinders.push_back(new CallbackBinderFw<TYPES>(mBinders.size()));
		return (typename FwBwTransportIf<TYPES>::fw_interface_type&) *mBinders[mBinders.size() - 1u];
	}

	virtual const typename FwBwTransportIf<TYPES>::fw_interface_type& get_base_interface() const /* = delete */{
		HV_ERR(
				"get_base_interface() const method is not allowed in multi-sockets")
		exit(EXIT_FAILURE);
		return base_type::get_base_interface();
	}

	//** Binding **//
	// Hierarchical binding
	virtual void bind(this_type& s) {
		s.setHierarchicalBind(this);
		mBoundHierarchically = true;
		s.base_type::bind(s.mDummy);
		base_type::bind(s);
	}

	virtual void operator()(this_type &s) {
		bind(s);
	}

	virtual void bind(base_type& s) /* = delete */{
		HV_ERR("Can't do hierarchical binding between multi/mono sockets")
		exit(EXIT_FAILURE);
	}

	virtual void operator()(base_type& s) /* = delete */{
		bind(s);
	}

protected:
	void setHierarchicalBind(this_type* s) {
		mHierarchicalBind = s;
	}

	this_type* getHierarchicalBind() {
		if (mHierarchicalBind) {
			return mHierarchicalBind->getHierarchicalBind();
		}
		return this;
	}

	void fillSocketList() {
		for(int i = 0u; i < base_type::get_base_port().size(); ++i) {
			mSockets.push_back(base_type::operator[](i));
		}
	}

public:
	void before_end_of_elaboration() {
		// Binding export only if this socket has not been bound hierarchically
		// (otherwise it is already bound)
		if (!mHierarchicalBind && !mBoundHierarchically) {
			base_type::bind(mDummy);
		}

		// If I'm bound hierarchically, nothing else to do
		if (mBoundHierarchically) {
			return;
		}

		// Else...
		std::vector<CallbackBinderFw<TYPES>*>& binders =
				getHierarchicalBind()->getBinders();
		if (binders.empty()) {
			HV_WARN("At least one callback was registered to multi-target socket but no socket was bound to it")
			return;
		} 

		// Getting sockets
		// 1 - Filling socket list at lowest level
		getHierarchicalBind()->fillSocketList();
		// 2 - Getting list
		mSockets = getHierarchicalBind()->getSockets();

		// Registering callbacks
		for (std::size_t i = 0; i < binders.size(); i++) {
			binders[i]->setBTransportCb(mBTransportCb);
			binders[i]->setNBTransportFwCb(mNBTransportFwCb);
			binders[i]->setTransportDbgCb(mTransportDbgCb);
			binders[i]->setGetDirectMemPtrCb(mGetDirectMemPtrCb);
		}

	}

	//** HELPERS **//
	std::size_t getNSockets() const {
		return mSockets.size();
	}

	std::vector<CallbackBinderFw<TYPES>*>& getBinders() {
		return mBinders;
	}

	std::vector<::tlm::tlm_bw_transport_if<TYPES>*>& getSockets() {
		return mSockets;
	}

	tlm::tlm_bw_transport_if<TYPES>* operator[](int i) {
		return mSockets[i];
	}

protected:
	std::vector<CallbackBinderFw<TYPES>*> mBinders;
	std::vector<::tlm::tlm_bw_transport_if<TYPES>*> mSockets;

	BTransportCb_type mBTransportCb;
	NBTransportFwCb_type mNBTransportFwCb;
	TransportDbgCb_type mTransportDbgCb;
	GetDirectMemPtrCb_type mGetDirectMemPtrCb;

	this_type* mHierarchicalBind;
	CallbackBinderFw<TYPES> mDummy;
	bool mBoundHierarchically;

};

}
}
}
}

#endif /* HV_UTILS_TLM_2_SOCKETS_MULTI_INITIATOR_SOCKET_H_ */
