/**
 * @file simple_target_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Convenient Initiator Socket
 *
 * This convenient socket accelerates development by providing the possibility to
 * register callbacks to overload interfaces methods.
 */

#ifndef HV_UTILS_TLM_2_SOCKETS_SIMPLE_TARGET_SOCKET_H_
#define HV_UTILS_TLM_2_SOCKETS_SIMPLE_TARGET_SOCKET_H_

#include <systemc>
#include <tlm>
#include <hv/common.h>

#include "../../../utils/tlm-2/sockets/simple_socket_base.h"
#include "../../../core/tlm-2/sockets/target_socket.h"
#include "../../../core/tlm-2/interfaces/fw_bw_transport_if.h"

namespace hv {
namespace communication {
namespace tlm2 {
namespace utils {

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N = 1, sc_core::sc_port_policy POL =
		sc_core::SC_ONE_OR_MORE_BOUND>
class SimpleTargetSocketB: public TargetSocket<BUSWIDTH, TYPES, N, POL>,
		protected SimpleSocketBase {
	friend class fw_process;
public:
	typedef typename TYPES::tlm_payload_type transaction_type;
	typedef typename TYPES::tlm_phase_type phase_type;
	typedef tlm::tlm_sync_enum sync_enum_type;
	typedef typename ::hv::communication::tlm2::FwBwTransportIf<TYPES>::fw_interface_type fw_interface_type;
	typedef TargetSocket<BUSWIDTH, TYPES, N, POL> base_type;

	typedef ::hv::common::CallbackImpl<
			sync_enum_type(transaction_type&, phase_type&, sc_core::sc_time&)> NBTransportPtr;
	typedef ::hv::common::CallbackImpl<
			void(transaction_type&, sc_core::sc_time&)> BTransportPtr;
	typedef ::hv::common::CallbackImpl<unsigned int(transaction_type&)> TransportDbgPtr;
	typedef ::hv::common::CallbackImpl<bool(transaction_type&, tlm::tlm_dmi&)> GetDirectMemPtr;

	explicit SimpleTargetSocketB(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("SimpleTargetSocket")) :
			base_type(name_), mFwProcess() {
		this->bind(mFwProcess);
	}

	//** Callback registration functions **//
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
			HV_WARN("No get_direct_mem_ptr callback registered")
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

private:
	fw_process mFwProcess;
};

template<unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N = 1, sc_core::sc_port_policy POL =
		sc_core::SC_ONE_OR_MORE_BOUND> class SimpleTargetSocket: public SimpleTargetSocketB<
		BUSWIDTH, TYPES, N, POL> {
	typedef SimpleTargetSocketB<BUSWIDTH, TYPES, N, POL> socket_b;
public:
	SimpleTargetSocket(::sc_core::sc_module_name name_ =
			sc_core::sc_gen_unique_name("SimpleTargetSocket")) :
			socket_b(name_) {
	}

};

}
}
}
}

#endif /* HV_UTILS_TLM_2_SOCKETS_SIMPLE_TARGET_SOCKET_H_ */
