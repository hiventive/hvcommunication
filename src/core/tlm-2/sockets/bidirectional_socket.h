/**
 * @file bidirectional_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM base bidirectional socket
 *
 * This socket has a port and an export
 * It can be bounded to a bidirectional socket or an initiator socket + a target socket
 */

#ifndef HV_CORE_TLM_2_SOCKETS_BIDIRECTIONAL_SOCKET_H_
#define HV_CORE_TLM_2_SOCKETS_BIDIRECTIONAL_SOCKET_H_

#include "core/tlm-2/sockets/initiator_socket.h"
#include "core/tlm-2/sockets/target_socket.h"
#include <systemc>
#include <tlm>
#include <HVCommon>
#include <core/tlm-2/base_protocol/base_protocol.h>
#include "core/tlm-2/interfaces/fw_bw_transport_if.h"

namespace hv {
namespace communication {
namespace tlm2 {

template <unsigned int BUSWIDTH = 32, typename FW_BW_IF = FwBwTransportIf<>, int N_INIT = 1,
          int N_TARG = 1, sc_core::sc_port_policy POL_INIT = sc_core::SC_ONE_OR_MORE_BOUND,
          sc_core::sc_port_policy POL_TARG = sc_core::SC_ONE_OR_MORE_BOUND>
class BaseBidirectionalSocket : public ::sc_core::sc_module {

  public:
    typedef FW_BW_IF fw_bw_interface_type;
    typedef typename FW_BW_IF::bw_interface_type bw_interface_type;
    typedef typename FW_BW_IF::fw_interface_type fw_interface_type;
    typedef BaseBidirectionalSocket<BUSWIDTH, FW_BW_IF, N_INIT, N_TARG, POL_INIT, POL_TARG>
        this_type;
    typedef BaseInitiatorSocket<BUSWIDTH, fw_interface_type, bw_interface_type, N_INIT, POL_INIT>
        base_initiator_type;
    typedef BaseTargetSocket<BUSWIDTH, fw_interface_type, bw_interface_type, N_TARG, POL_TARG>
        base_target_type;

    BaseBidirectionalSocket(base_initiator_type &init_, base_target_type &targ_)
        : ::sc_core::sc_module(::sc_core::sc_gen_unique_name("BaseBidirectionalSocket")),
          mInitiatorBase(init_), mTargetBase(targ_) {}

    explicit BaseBidirectionalSocket(::sc_core::sc_module_name name_, base_initiator_type &init_,
                                     base_target_type &targ_)
        : ::sc_core::sc_module(name_), mInitiatorBase(init_), mTargetBase(targ_) {}

    //** Interface binding **//

    /**
     * FW interface binding (to target)
     * @param ifs FW Interface
     */
    virtual void bind(fw_interface_type &ifs) { mTargetBase.bind(ifs); }

    virtual void operator()(fw_interface_type &ifs) { bind(ifs); }

    /**
     * BW interface binding (to initiator)
     * @param ifs BW Interface
     */
    virtual void bind(bw_interface_type &ifs) { mInitiatorBase.bind(ifs); }

    virtual void operator()(bw_interface_type &ifs) { bind(ifs); }

    /**
     * FW and BW interface binding
     * @param ifs FW/BW Interface
     */
    virtual void bind(fw_bw_interface_type &ifs) {
        bind((fw_interface_type &)ifs);
        bind((bw_interface_type &)ifs);
    }

    virtual void operator()(fw_bw_interface_type &ifs) { bind(ifs); }

    //** Socket Binding **//
    /**
     * Binding to Bidirectional socket
     * @param s Socket to bind to
     *
     * This class is template to support binding between
     * multiple policies
     */

    template <int N_INIT2, int N_TARG2, sc_core::sc_port_policy POL_INIT2,
              sc_core::sc_port_policy POL_TARG2>
    void
    bind(BaseBidirectionalSocket<BUSWIDTH, FW_BW_IF, N_INIT2, N_TARG2, POL_INIT2, POL_TARG2> &s) {
        mInitiatorBase.bind(s.target());
        s.initiator().bind(mTargetBase);
    }

    template <int N_INIT2, int N_TARG2, sc_core::sc_port_policy POL_INIT2,
              sc_core::sc_port_policy POL_TARG2>
    void operator()(
        BaseBidirectionalSocket<BUSWIDTH, FW_BW_IF, N_INIT2, N_TARG2, POL_INIT2, POL_TARG2> &s) {
        bind(s);
    }

    virtual void bind(base_initiator_type &s) { s.bind(mTargetBase); }

    virtual void operator()(base_initiator_type &s) { bind(s); }

    virtual void bindHierarchical(base_initiator_type &s) { mInitiatorBase.bind(s); }

    virtual void bind(base_target_type &s) { mInitiatorBase.bind(s); }

    virtual void operator()(base_target_type &s) { bind(s); }

    virtual void bindHierarchical(base_target_type &s) { s.bind(mTargetBase); }

    /**
     * Hierarchical binding to Bidirectional socket
     * @param s Socket to bind to
     *
     * This class is template to support binding between
     * multiple policies
     */
    template <int N_INIT2, int N_TARG2, sc_core::sc_port_policy POL_INIT2,
              sc_core::sc_port_policy POL_TARG2>
    void bindHierarchical(
        BaseBidirectionalSocket<BUSWIDTH, FW_BW_IF, N_INIT2, N_TARG2, POL_INIT2, POL_TARG2> &s) {
        mInitiatorBase.bind(s.initiator());
        s.target().bind(mTargetBase);
    }

    //** Initiator/Target accessors **//
    base_initiator_type &initiator() { return mInitiatorBase; }

    base_target_type &target() { return mTargetBase; }

    //** Implicit cast **//
    // Bidirectional socket casts into target if implicit
    operator base_target_type &() { return mTargetBase; }

    explicit operator base_initiator_type &() { return mInitiatorBase; }

  protected:
    base_initiator_type &mInitiatorBase;
    base_target_type &mTargetBase;
};

template <unsigned int BUSWIDTH = 32, typename TYPES = BaseProtocolTypes, int N_INIT = 1,
          int N_TARG = 1, sc_core::sc_port_policy POL_INIT = sc_core::SC_ONE_OR_MORE_BOUND,
          sc_core::sc_port_policy POL_TARG = sc_core::SC_ONE_OR_MORE_BOUND>
class BidirectionalSocket : public BaseBidirectionalSocket<BUSWIDTH, FwBwTransportIf<TYPES>, N_INIT,
                                                           N_TARG, POL_INIT, POL_TARG> {
    typedef typename FwBwTransportIf<TYPES>::fw_interface_type fw_interface_type;
    typedef typename FwBwTransportIf<TYPES>::bw_interface_type bw_interface_type;
    typedef FwBwTransportIf<TYPES> fw_bw_interface_type;
    typedef BaseBidirectionalSocket<BUSWIDTH, FwBwTransportIf<TYPES>, N_INIT, N_TARG, POL_INIT,
                                    POL_TARG>
        base_type;
    typedef InitiatorSocket<BUSWIDTH, TYPES, N_INIT, POL_INIT> initiator_type;
    typedef TargetSocket<BUSWIDTH, TYPES, N_INIT, POL_INIT> target_type;

  public:
    BidirectionalSocket(
        ::sc_core::sc_module_name name_ = ::sc_core::sc_gen_unique_name("BidirectionalSocket"))
        : base_type(name_, mInitiator, mTarget), mInitiator("Initiator"), mTarget("Target"),
          mBidirectionalPort(base_type::mInitiatorBase, base_type::mTargetBase) {}

    virtual ~BidirectionalSocket() {}

  protected:
    class BidirectionalPort {
      public:
        typedef typename TYPES::tlm_payload_type payload_type;
        typedef typename TYPES::tlm_phase_type phase_type;

        BidirectionalPort(typename base_type::base_initiator_type &initiatorPorts_,
                          typename base_type::base_target_type &targetPorts_)
            : mIndex(0), mInitiatorBasePorts(initiatorPorts_), mTargetBasePorts(targetPorts_) {}

        virtual ~BidirectionalPort() {}

        //** FW Interface **//
        void b_transport(payload_type &txn, sc_core::sc_time &t) {
            mInitiatorBasePorts[mIndex]->b_transport(txn, t);
        }

        ::tlm::tlm_sync_enum nb_transport_fw(payload_type &txn, phase_type &p,
                                             sc_core::sc_time &t) {
            return mInitiatorBasePorts[mIndex]->nb_transport_fw(txn, p, t);
        }

        bool get_direct_mem_ptr(payload_type &trans, ::tlm::tlm_dmi &dmi_data) {
            return mInitiatorBasePorts[mIndex]->get_direct_memory_ptr(trans, dmi_data);
        }

        unsigned int transport_dbg(payload_type &trans) {
            return mInitiatorBasePorts[mIndex]->transport_dbg(trans);
        }

        //** BW Interface **//
        ::tlm::tlm_sync_enum nb_transport_bw(payload_type &txn, phase_type &p,
                                             sc_core::sc_time &t) {
            return mTargetBasePorts[mIndex]->nb_transport_bw(txn, p, t);
        }

        void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range) {
            mTargetBasePorts[mIndex]->invalidate_direct_mem_ptr(start_range, end_range);
        }

        int mIndex;

      protected:
        typename base_type::base_initiator_type &mInitiatorBasePorts;
        typename base_type::base_target_type &mTargetBasePorts;
    };

  public:
    BidirectionalPort *operator[](const int &i) {
        mBidirectionalPort.mIndex = i;
        return &mBidirectionalPort;
    }

    BidirectionalPort *operator->() { return this->operator[](0); }

    virtual sc_core::sc_type_index get_protocol_types() const { return typeid(TYPES); }

  protected:
    initiator_type mInitiator;
    target_type mTarget;
    BidirectionalPort mBidirectionalPort;
};

} // namespace tlm2
} // namespace communication
} // namespace hv

#endif // HV_CORE_TLM_2_SOCKETS_BIDIRECTIONAL_SOCKET_H_
