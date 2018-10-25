/**
 * @file protocol_adapter.h
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

#ifndef SRC_CORE_TLM_2_PROTOCOL_ADAPTER_PROTOCOL_ADAPTER_H_
#define SRC_CORE_TLM_2_PROTOCOL_ADAPTER_PROTOCOL_ADAPTER_H_

#include "../../../core/tlm-2/sockets/initiator_socket.h"
#include "../../../core/tlm-2/sockets/target_socket.h"
#include "../../../core/tlm-2/protocol_adapter/protocol_converter_if.h"

namespace hv {
namespace communication {
namespace tlm2 {

// Helper struct for implicit template polymorphism
template<typename TYPES_IN, typename TYPES_OUT, bool CONV_DIRECTION> class ConverterDirectionSolver {
};

template<typename TYPES_IN, typename TYPES_OUT> class ConverterDirectionSolver<
		TYPES_IN, TYPES_OUT, true> {
public:
	typedef ProtocolConverterIf<TYPES_IN, TYPES_OUT> pc_type;
	ConverterDirectionSolver(pc_type& mPC_) :
			mPC(mPC_) {

	}
	void deepCopy(const typename pc_type::payload_in_type& src,
			typename pc_type::payload_out_type& dst) {
		mPC.deepCopy(src, dst);
	}
	void deepCopy(const typename pc_type::payload_out_type& src,
			typename pc_type::payload_in_type& dst) {
		mPC.deepCopy(src, dst);
	}
protected:
	pc_type& mPC;
};

template<typename TYPES_IN, typename TYPES_OUT> class ConverterDirectionSolver<
		TYPES_IN, TYPES_OUT, false> {
public:
	typedef ProtocolConverterIf<TYPES_OUT, TYPES_IN> pc_type;
	ConverterDirectionSolver(pc_type& mPC_) :
			mPC(mPC_) {

	}
	void deepCopy(const typename pc_type::payload_out_type& src,
			typename pc_type::payload_in_type& dst) {
		mPC.deepCopy(src, dst);
	}
	void deepCopy(const typename pc_type::payload_in_type& src,
			typename pc_type::payload_out_type& dst) {
		mPC.deepCopy(src, dst);
	}
protected:
	pc_type& mPC;
};

template<typename TYPES_IN, int N_IN, sc_core::sc_port_policy POL_IN,
		typename TYPES_OUT, int N_OUT, sc_core::sc_port_policy POL_OUT,
		bool CONVERSION_DIRECTION = true, unsigned int BUSWIDTH = 32> class ProtocolAdapter: public ::sc_core::sc_module {
public:
	typedef typename TYPES_OUT::tlm_payload_type payload_output_type;
	typedef typename TYPES_OUT::tlm_phase_type phase_output_type;
	typedef typename TYPES_IN::tlm_payload_type payload_input_type;
	typedef typename TYPES_IN::tlm_phase_type phase_input_type;

	typedef ::hv::communication::tlm2::TargetSocket<BUSWIDTH, TYPES_IN, N_IN, POL_IN> input_socket_type;
	typedef ::hv::communication::tlm2::InitiatorSocket<BUSWIDTH, TYPES_OUT, N_OUT, POL_OUT> output_socket_type;

	typedef ::hv::communication::tlm2::FwBwTransportIf<TYPES_OUT> fw_bw_transport_if_output_type;
	typedef typename fw_bw_transport_if_output_type::fw_interface_type fw_transport_if_output_type;
	typedef typename fw_bw_transport_if_output_type::bw_interface_type bw_transport_if_output_type;

	typedef ::hv::communication::tlm2::FwBwTransportIf<TYPES_IN> fw_bw_transport_if_input_type;
	typedef typename fw_bw_transport_if_input_type::fw_interface_type fw_transport_if_input_type;
	typedef typename fw_bw_transport_if_input_type::bw_interface_type bw_transport_if_input_type;

	typedef tlm::tlm_sync_enum sync_enum_type;

	ProtocolAdapter(::sc_core::sc_module_name name_,
			typename ConverterDirectionSolver<TYPES_IN, TYPES_OUT, CONVERSION_DIRECTION>::pc_type& protocolConverter) :
			::sc_core::sc_module(name_), inputSocket("InputSocket"), outputSocket(
					"OutputSocket"), inputSocketClog("InputSocketClog"), outputSocketClog(
					"OutputSocketClog"), mBwIf(*this), mFwIf(*this), mProtocolConverter(
					protocolConverter) {

		inputSocket.bind((fw_transport_if_input_type&) mFwIf);
		outputSocket.bind((bw_transport_if_output_type&) mBwIf);

		inputSocketClog.bind((bw_transport_if_input_type&) mBwIfClog);
		outputSocketClog.bind((fw_transport_if_output_type&) mFwIfClog);

	}

	virtual ~ProtocolAdapter() {
	}

	::std::size_t howManyInputBindings() const {
		return inputSocket.size();
		// FIXME: check if it has the right behavior
	}

	::std::size_t howManyOutputBindings() const {
		return outputSocket.size();
		// FIXME: check if it has the right behavior
	}

	input_socket_type inputSocket;
	output_socket_type outputSocket;

	//** Clogs **//
	::hv::communication::tlm2::InitiatorSocket<BUSWIDTH, TYPES_IN, 1,
			::sc_core::SC_ZERO_OR_MORE_BOUND> inputSocketClog;
	::hv::communication::tlm2::TargetSocket<BUSWIDTH, TYPES_OUT, 1,
			::sc_core::SC_ZERO_OR_MORE_BOUND> outputSocketClog;

protected:

//** Interface classes**//
	struct bw_interface_type_output: public bw_transport_if_output_type {
		bw_interface_type_output(ProtocolAdapter& owner_) :
				owner(owner_) {

		}

		sync_enum_type nb_transport_bw(payload_output_type& trans,
				phase_output_type& phase, sc_core::sc_time& t) {
			payload_input_type transInput;
			owner.mProtocolConverter.deepCopy(trans, transInput);
			phase_input_type phaseInput = phase;
			sync_enum_type ret = owner.inputSocket->nb_transport_bw(transInput,
					phaseInput, t);
			owner.mProtocolConverter.deepCopy(transInput, trans);
			phase = phaseInput;
			return ret;

		}

		void invalidate_direct_mem_ptr(sc_dt::uint64 startRange,
				sc_dt::uint64 endRange) {
			owner.inputSocket->invalidate_direct_mem_ptr(startRange, endRange);
		}

		ProtocolAdapter& owner;
	};

	struct bw_interface_type_input: public bw_transport_if_input_type {
		// This is a foo interface for clogging
		sync_enum_type nb_transport_bw(payload_input_type& trans,
				phase_input_type& phase, sc_core::sc_time& t) {
			return sync_enum_type::TLM_ACCEPTED;
		}

		void invalidate_direct_mem_ptr(sc_dt::uint64 startRange,
				sc_dt::uint64 endRange) {
		}
	};

	struct fw_interface_type_input: public fw_transport_if_input_type {
		fw_interface_type_input(ProtocolAdapter& owner_) :
				owner(owner_) {

		}
		sync_enum_type nb_transport_fw(payload_input_type& trans,
				phase_input_type& phase, sc_core::sc_time& t) {
			payload_output_type transOutput;
			owner.mProtocolConverter.deepCopy(trans, transOutput);
			phase_output_type phaseOutput = phase;
			sync_enum_type ret = owner.outputSocket->nb_transport_fw(
					transOutput, phaseOutput, t);
			owner.mProtocolConverter.deepCopy(transOutput, trans);
			phase = phaseOutput;
			return ret;
		}

		void b_transport(payload_input_type& trans, sc_core::sc_time& t) {
			payload_output_type transOutput;
			owner.mProtocolConverter.deepCopy(trans, transOutput);
			owner.outputSocket->b_transport(transOutput, t);
			owner.mProtocolConverter.deepCopy(transOutput, trans);
		}

		unsigned int transport_dbg(payload_input_type& trans) {
			payload_output_type transOutput;
			owner.mProtocolConverter.deepCopy(trans, transOutput);
			unsigned int ret = owner.outputSocket->transport_dbg(transOutput);
			owner.mProtocolConverter.deepCopy(transOutput, trans);
			return ret;
		}

		bool get_direct_mem_ptr(payload_input_type& trans,
				tlm::tlm_dmi& dmiData) {
			payload_output_type transOutput;
			owner.mProtocolConverter.deepCopy(trans, transOutput);
			unsigned int ret = owner.outputSocket->get_direct_mem_ptr(
					transOutput, dmiData);
			owner.mProtocolConverter.deepCopy(transOutput, trans);
			return ret;

		}

		ProtocolAdapter& owner;
	};

	struct fw_interface_type_output: public fw_transport_if_output_type {
		// This is a foo interface for clogging
		sync_enum_type nb_transport_fw(payload_output_type& trans,
				phase_output_type& phase, sc_core::sc_time& t) {
			return sync_enum_type::TLM_ACCEPTED;
		}

		void b_transport(payload_output_type& trans, sc_core::sc_time& t) {
		}

		unsigned int transport_dbg(payload_output_type& trans) {
			return 0u;
		}

		bool get_direct_mem_ptr(payload_output_type& trans,
				tlm::tlm_dmi& dmiData) {
			return true;
		}

	};

	//** Interface objects **//
	bw_interface_type_output mBwIf;
	fw_interface_type_input mFwIf;
	bw_interface_type_input mBwIfClog;
	fw_interface_type_output mFwIfClog;

	//** Protocol converter **//
	ConverterDirectionSolver<TYPES_IN, TYPES_OUT, CONVERSION_DIRECTION> mProtocolConverter;
};

}
}
}

#endif /* SRC_CORE_TLM_2_PROTOCOL_ADAPTER_PROTOCOL_ADAPTER_H_ */
