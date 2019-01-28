#include <iostream>
#include <systemc>
#include <HVConfiguration>

#include "tlm-initiator.h"
#include "tlm-target.h"
#include "hv-initiator.h"
#include "hv-target.h"

int sc_main(int argc, char *argv[]) {
	hv::cfg::Broker hiventiveBroker("Hiventive broker");

	// TLM -> TLM
	TLMInitiator tlmInitiatorTLMTLM("Initiator-TLM-TLM");
	TLMTarget tlmTargetTLMTLM("Target-TLM-TLM");
	tlmInitiatorTLMTLM.socket.bind(tlmTargetTLMTLM.socket);

	// HV -> HV
	HVInitiator hvInitiatorHVHV("Initiator-HV-HV");
	HVTarget hvTargetHVHV("Target-HV-HV");
	hvInitiatorHVHV.socket.bind(hvTargetHVHV.socket);

	// HV -> TLM
	HVInitiator hvInitiatorHVTLM("Initiator-HV-TLM");
	TLMTarget tlmTargetHVTLM("Target-HV-TLM");
	hvInitiatorHVTLM.socket.bind(tlmTargetHVTLM.socket);

	// TLM -> HV
	TLMInitiator tlmInitiatorTLMHV("Initiator-TLM-HV");
	HVTarget hvTargetTLMHV("Target-TLM-HV");
	tlmInitiatorTLMHV.socket.bind(hvTargetTLMHV.socket);

	::sc_core::sc_start();

	return 0;
}
