/**
 * @file multi_initiator_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Utils Multi Initiator Socket
 */

#ifndef HV_UTILS_TLM_2_SOCKETS_MULTI_INITIATOR_SOCKET_H_
#define HV_UTILS_TLM_2_SOCKETS_MULTI_INITIATOR_SOCKET_H_

#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/sockets/target_socket.h"
#include "multi_socket_callback_binder_bw.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

// MultiTargetSocket forward declaration
template<unsigned int BUSWIDTH, typename TYPES, int N,
		sc_core::sc_port_policy POL> class MultiTargetSocket;

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N =
		0, sc_core::sc_port_policy POL = sc_core::SC_ONE_OR_MORE_BOUND> class MultiInitiatorSocket: public InitiatorSocket<
		BUSWIDTH, TYPES, N, POL> {
public:
	typedef MultiInitiatorSocket<BUSWIDTH, TYPES, N, POL> this_type;
	typedef InitiatorSocket<BUSWIDTH, TYPES, N, POL> base_type;
	typedef typename base_type::base_target_socket_type base_target_socket_type;
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;
	typedef typename CallbackBinderBw<TYPES>::NBTransportBwCb NBTransportBwCb_type;
	typedef typename CallbackBinderBw<TYPES>::InvalidateDirectMemPtrCb InvalidateDirectMemPtrCb_type;

	using base_type::bind;

	MultiInitiatorSocket(
			const std::string &name_ = sc_core::sc_gen_unique_name(
					"MultiInitiatorSocket")) :
			base_type(name_.c_str()), mNBTransportBwCb(nullptr), mInvalidateDirectMemPtrCb(
					nullptr), mHierarchicalBind(nullptr), mDummy(2713u), mBoundHierarchically(
					false) {
	}

	virtual ~MultiInitiatorSocket() {
		for (auto it = mBinders.begin(); it != mBinders.end(); ++it) {
			delete *it;
		}
	}

	//** Callback registration **//
	void registerNBTransportBw(const NBTransportBwCb_type &cb) {
		if (mNBTransportBwCb) {
			HV_WARN(
					"Callback already registered for nb_transport_bw. Ignoring new registration")
			return;
		}
		mNBTransportBwCb = cb;
	}

	template<typename MODULE> void registerNBTransportBw(MODULE* mod,
			sync_enum_type (MODULE::*cb)(const std::size_t &, transaction_type&,
					phase_type&, sc_core::sc_time&)) {
		NBTransportBwCb_type cbTmp(cb, mod);
		registerNBTransportBw(cbTmp);
	}

	void registerInvalidateDirectMemPtr(
			const InvalidateDirectMemPtrCb_type &cb) {
		if (mInvalidateDirectMemPtrCb) {
			HV_WARN(
					"Callback already registered for invalidate_direct_mem_ptr. Ignoring new registration")
			return;
		}
		mInvalidateDirectMemPtrCb = cb;
	}

	template<typename MODULE> void registerInvalidateDirectMemPtr(MODULE* mod,
			void (MODULE::*cb)(const std::size_t &, sc_dt::uint64,
					sc_dt::uint64)) {
		InvalidateDirectMemPtrCb_type cbTmp(cb, mod);
		registerInvalidateDirectMemPtr(cbTmp);
	}

	//** Get base interface **//
	// Overloaded to generate a new interface at each call
	virtual typename FwBwTransportIf<TYPES>::bw_interface_type& get_base_interface() {
		mBinders.push_back(new CallbackBinderBw<TYPES>(mBinders.size()));
		return (typename FwBwTransportIf<TYPES>::bw_interface_type&) *mBinders[mBinders.size()
				- 1u];
	}

	// Disabling get_base_interface_const as creating a new interface is impossible
	virtual const typename FwBwTransportIf<TYPES>::bw_interface_type& get_base_interface() const /* = delete */{
		HV_ERR(
				"get_base_interface() const method is not allowed in multi-sockets")
		exit(EXIT_FAILURE);
		return base_type::get_base_interface();
	}

	virtual sc_core::sc_export<
			typename FwBwTransportIf<TYPES>::bw_interface_type>& get_base_export() {
		if (!mBoundHierarchically) //we are not bound hierarchically
			base_type::m_export.bind(mDummy); //so we bind the dummy to avoid a SystemC error
		return base_type::get_base_export(); //and then return our own export so that the hierarchical binding is set up properly
	}

	virtual const sc_core::sc_export<
			typename FwBwTransportIf<TYPES>::bw_interface_type>& get_base_export() const {
		return base_type::get_base_export();
	}

	//** BINDING **//
	// Hierarchical binding
	virtual void bind(this_type& s) {
		// In case of hierarchical binding, we suppose it is always the inside binding
		// to the outside. Therefore, we are the inside. We just notify the outside to redirect
		// everything to us
		mHierarchicalBind = &s;
		s.notifyHierarchicalBinding();
		base_type::bind(s);
	}

	virtual void operator()(this_type& s) {
		bind(s);
	}

	virtual void bind(base_type& s) {
		HV_ERR("Can't do hierarchical binding between multi/mono sockets")
		exit(EXIT_FAILURE);
	}

	void notifyHierarchicalBinding() {
		mBoundHierarchically = true;
	}

	// Overloading bind with target socket (not applicable to MultiTargetSocket)
	virtual void bind(base_target_socket_type& s) {
		if (mHierarchicalBind) {
			mHierarchicalBind->bind(s);
		} else {
			// Calling parent bind
			base_type::bind(s);
			// Adding to my socket's list
			mSockets.push_back(&s);
		}
	}

	virtual void operator()(base_target_socket_type& s) {
		bind(s);
	}

	/**
	 * Bind returning assigned ID
	 * @param s Socket to bind
	 * @param id ID assigned to socket
	 */
	virtual void bind(base_target_socket_type& s, std::size_t *id) {
		bind(s);
		*id = mSockets.size() - 1u;
	}

	virtual void operator()(base_target_socket_type& s, std::size_t *id) {
		bind(s, id);
	}

	//** Before end of elaboration (must be public) **//
	void before_end_of_elaboration() {
		// Binding export if not bound to avoid SystemC error
		if (!base_type::m_export.get_interface()) {
			base_type::m_export.bind(mDummy);
		}

		// If I am bound hierarchically, I'm not binding callbacks
		if (mBoundHierarchically)
			return;

		// Else I do
		std::vector<CallbackBinderBw<TYPES>*>& binders =
				getHierarchicalBind()->getBinders();

		// Getting sockets
		mSockets = getHierarchicalBind()->getSockets();

		// Registering callbacks
		for (std::size_t i = 0; i < binders.size(); i++) {
			binders[i]->setNBTransportBwCb(mNBTransportBwCb);
			binders[i]->setInvalidateDirectMemPtrCb(mInvalidateDirectMemPtrCb);
		}
	}

	typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type* operator[](
			int i) {
		return &(mSockets[i]->get_base_interface());
	}

protected:
	this_type* getHierarchicalBind() {
		if (mHierarchicalBind) {
			return mHierarchicalBind->getHierarchicalBind();
		}
		return this;

	}

public:
	//** HELPERS **//
	std::size_t getNSockets() const {
		return mSockets.size();
	}

	std::vector<CallbackBinderBw<TYPES>*>& getBinders() {
		return mBinders;
	}

	std::vector<base_target_socket_type*>& getSockets() {
		return mSockets;
	}

protected:
	std::vector<CallbackBinderBw<TYPES>*> mBinders;
	std::vector<base_target_socket_type*> mSockets;
	NBTransportBwCb_type mNBTransportBwCb;
	InvalidateDirectMemPtrCb_type mInvalidateDirectMemPtrCb;
	this_type* mHierarchicalBind;
	CallbackBinderBw<TYPES> mDummy;
	bool mBoundHierarchically;
};

}
}
}
}

#endif /* HV_UTILS_TLM_2_SOCKETS_MULTI_INITIATOR_SOCKET_H_ */
