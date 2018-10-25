/**
 * @file simple_bidirectional_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Simple Bidirectional Socket
 */

#ifndef HV_UTILS_TLM_2_SOCKETS_SIMPLE_BIDIRECTIONAL_SOCKET_H_
#define HV_UTILS_TLM_2_SOCKETS_SIMPLE_BIDIRECTIONAL_SOCKET_H_

#include <tlm>
#include <hv/common.h>

#include "../../../utils/tlm-2/sockets/simple_socket_base.h"
#include "../../../core/tlm-2/sockets/bidirectional_socket.h"
#include "../../../core/tlm-2/interfaces/fw_bw_transport_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes,
		int N_INIT = 1, int N_TARG = 1, sc_core::sc_port_policy POL_INIT =
				sc_core::SC_ONE_OR_MORE_BOUND,
		sc_core::sc_port_policy POL_TARG = sc_core::SC_ONE_OR_MORE_BOUND> class SimpleBidirectionalSocketB: public BidirectionalSocket<
		BUSWIDTH, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG>,
		protected SimpleSocketBase {
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;
	typedef typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type fw_interface_type;
	typedef typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::bw_interface_type bw_interface_type;
	typedef BidirectionalSocket<BUSWIDTH, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG> base_type;
public:
	// BW callbacks
	typedef ::hv::common::CallbackImpl<
			sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)> TransportCb;
	typedef ::hv::common::CallbackImpl<void(sc_dt::uint64, sc_dt::uint64)> InvalidateDirectMemPtrCb;
	// FW callbacks
	typedef ::hv::common::CallbackImpl<
			sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)> NBTransportPtr;
	typedef ::hv::common::CallbackImpl<
			void(transaction_type&, sc_core::sc_time&)> BTransportPtr;
	typedef ::hv::common::CallbackImpl<unsigned int(transaction_type&)> TransportDbgPtr;
	typedef ::hv::common::CallbackImpl<bool(transaction_type&, tlm::tlm_dmi&)> GetDirectMemPtr;

	SimpleBidirectionalSocketB(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("SimpleBidirectionalSocket")) :
			base_type(name_), mFwProcess() {
		this->bind(mFwProcess);
		this->bind(mBwProcess);
	}

	//** Callback registration **//

	// BW interface
	void registerNBTransportBw(const TransportCb &cb) {
		mBwProcess.setTransportPtr(cb);
	}

	template<typename MODULE> void registerNBTransportBw(MODULE* mod,
			sync_enum_type (MODULE::*cb)(transaction_type&, phase_type&,
					sc_core::sc_time&)) {
		TransportCb cbTmp(cb, mod);
		this->registerNBTransportBw(cbTmp);
	}

	void registerInvalidateDirectMemPtr(const InvalidateDirectMemPtrCb &cb) {
		mBwProcess.setInvalidateDirectMemPtr(cb);
	}

	template<typename MODULE> void registerInvalidateDirectMemPtr(MODULE *mod,
			void (MODULE::*cb)(sc_dt::uint64, sc_dt::uint64)) {
		InvalidateDirectMemPtrCb cbTmp(cb, mod);
		this->registerInvalidateDirectMemPtr(cbTmp);
	}

	// FW interface
	void registerNBTransportFw(const NBTransportPtr &cb) {
		mFwProcess.setNBTransportPtr(cb);
	}

	template<typename MODULE> void registerNBTransportFw(MODULE* mod,
			sync_enum_type (MODULE::*cb)(transaction_type&, phase_type&,
					sc_core::sc_time&)) {
		NBTransportPtr cbTmp(cb, mod);
		this->registerNBTransportFw(cbTmp);
	}

	void registerBTransport(const BTransportPtr &cb) {
		mFwProcess.setBTransportPtr(cb);
	}

	template<typename MODULE> void registerBTransport(MODULE* mod,
			void (MODULE::*cb)(transaction_type&, sc_core::sc_time&)) {
		BTransportPtr cbTmp(cb, mod);
		this->registerBTransport(cbTmp);
	}

	void registerTransportDbg(const TransportDbgPtr &cb) {
		mFwProcess.setTransportDbgPtr(cb);
	}

	template<typename MODULE> void registerTransportDbg(MODULE* mod,
			unsigned int (MODULE::*cb)(transaction_type&)) {
		TransportDbgPtr cbTmp(cb, mod);
		this->registerTransportDbg(cbTmp);
	}

	void registerGetDirectMemPtr(const GetDirectMemPtr &cb) {
		mFwProcess.setGetDirectMemPtr(cb);
	}

	template<typename MODULE> void registerGetDirectMemPtr(MODULE* mod,
			bool (MODULE::*cb)(transaction_type&, tlm::tlm_dmi&)) {
		GetDirectMemPtr cbTmp(cb, mod);
		this->registerGetDirectMemPtr(cbTmp);
	}

private:
	class bw_process: public ::hv::communication::tlm2::FwBwTransportIf<TYPES>::bw_interface_type {
	public:

		bw_process() :
				mTransportPtr(nullptr), mInvalidateDirectMemPtr(nullptr) {
		}

		void setTransportPtr(const TransportCb &p) {
			if (mTransportPtr) {
				HV_WARN(
						"Non-blocking callback already registered. Ignoring new registration")
				return;
			}
			mTransportPtr = p;
		}

		void setInvalidateDirectMemPtr(const InvalidateDirectMemPtrCb &p) {
			if (mInvalidateDirectMemPtr) {
				HV_WARN(
						"Invalidate DMI callback already registered. Ignoring new registration");
				return;
			}
			mInvalidateDirectMemPtr = p;
		}

		//** Interface implementation **//
		sync_enum_type nb_transport_bw(transaction_type& trans,
				phase_type& phase, sc_core::sc_time& t) {
			if (mTransportPtr) {
				return mTransportPtr(trans, phase, t);
			}
			HV_ERR("No transport callback registered");
			return tlm::TLM_COMPLETED;
		}
		void invalidate_direct_mem_ptr(sc_dt::uint64 start_range,
				sc_dt::uint64 end_range) {
			if (mInvalidateDirectMemPtr) {
				// forward call
				mInvalidateDirectMemPtr(start_range, end_range);
			}
		}

	private:
		TransportCb mTransportPtr;
		InvalidateDirectMemPtrCb mInvalidateDirectMemPtr;

	};

	class fw_process: public ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type {
	public:
		fw_process() :
				mNBTransportPtr(nullptr), mBTransportPtr(nullptr), mTransportDbgPtr(
						nullptr), mGetDirectMemPtr(nullptr) {
		}

		void setNBTransportPtr(const NBTransportPtr &cb) {
			if (mNBTransportPtr) {
				HV_WARN("Non-blocking callback already registered. Ignoring")
				return;
			}
			mNBTransportPtr = cb;
		}

		void setBTransportPtr(const BTransportPtr &cb) {
			if (mBTransportPtr) {
				HV_WARN("Blocking callback already registered. Ignoring")
				return;
			}
			mBTransportPtr = cb;
		}

		void setTransportDbgPtr(const TransportDbgPtr &cb) {
			if (mTransportDbgPtr) {
				HV_WARN("Debug callback already registered. Ignoring")
				return;
			}
			mTransportDbgPtr = cb;
		}

		void setGetDirectMemPtr(const GetDirectMemPtr &cb) {
			if (mGetDirectMemPtr) {
				HV_WARN("Get DMI pointer callback already registered. Ignoring")
				return;
			}
			mGetDirectMemPtr = cb;
		}

		//** FW Interface Implementation **//
		sync_enum_type nb_transport_fw(transaction_type& trans,
				phase_type& phase, sc_core::sc_time& t) {
			if (mNBTransportPtr) {
				return mNBTransportPtr(trans, phase, t);
			}
			HV_ERR("No non-blocking transport callback registered")
			return tlm::TLM_COMPLETED;
		}

		void b_transport(transaction_type& trans, sc_core::sc_time& t) {
			if (mBTransportPtr) {
				mBTransportPtr(trans, t);
				return;
			}
			HV_ERR("No blocking transport callback registered")
		}

		unsigned int transport_dbg(transaction_type& trans) {
			if (mTransportDbgPtr) {
				return mTransportDbgPtr(trans);
			}
			// Just warning here. This should not cause failure.
			HV_WARN("No debug callback registered")
			return 0u;
		}

		bool get_direct_mem_ptr(transaction_type& trans,
				tlm::tlm_dmi& dmi_data) {
			if (mGetDirectMemPtr) {
				return mGetDirectMemPtr(trans, dmi_data);
			}
			dmi_data.allow_read_write();
			dmi_data.set_start_address(0x0);
			dmi_data.set_end_address((sc_dt::uint64) -1);
			return false;
		}

	private:
		NBTransportPtr mNBTransportPtr;
		BTransportPtr mBTransportPtr;
		TransportDbgPtr mTransportDbgPtr;
		GetDirectMemPtr mGetDirectMemPtr;
	};

	bw_process mBwProcess;
	fw_process mFwProcess;
};

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes,
		int N_INIT = 1, int N_TARG = 1, sc_core::sc_port_policy POL_INIT =
				sc_core::SC_ONE_OR_MORE_BOUND,
		sc_core::sc_port_policy POL_TARG = sc_core::SC_ONE_OR_MORE_BOUND> class SimpleBidirectionalSocket: public SimpleBidirectionalSocketB<
		BUSWIDTH, TYPES, N_INIT, N_TARG, POL_INIT, POL_TARG> {
	typedef SimpleBidirectionalSocketB<BUSWIDTH, TYPES, N_INIT, N_TARG,
			POL_INIT, POL_TARG> socket_b;
public:
	SimpleBidirectionalSocket(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("SimplBidirectionalSocket")) :
			socket_b(name_) {
	}

};

}
}
}
}

#endif /* SRC_UTILS_TLM_2_SOCKETS_SIMPLE_BIDIRECTIONAL_SOCKET_H_ */
