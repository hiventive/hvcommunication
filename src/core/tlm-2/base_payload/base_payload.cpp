/**
 * @file base_payload.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Base Payload
 *
 * Hiventive base payload should be inherited from any protocol payload
 * It embeds extension mechanism and memory management interface
 * This base payload is inspired by TLM 2.0 generic payload and can be viewed as
 * a minimalistic general-purpose payload
 */

#include "../../../core/tlm-2/base_payload/base_payload.h"

namespace hv {
namespace communication {
namespace tlm2 {

BasePayload::BasePayload() :
		Extensions(nullptr), mMMCount(0u) {
}

BasePayload::BasePayload(MemoryManagementIf* MMIn) :
		Extensions(nullptr), mMMCount(0u) {
}

BasePayload::~BasePayload() {

}

void BasePayload::acquire() {
	HV_ASSERT(mMM != nullptr, "Can't acquire non-memory-managed payload");
	mMMCount++;
}

void BasePayload::release() {
	HV_ASSERT(mMM != nullptr, "Can't release a non-memory-managed payload");
	HV_ASSERT(mMMCount > 0, "C'est release a non-acquired payload");
	if (--mMMCount == 0) {
		mMM->free(this);
	}
}

std::size_t BasePayload::getMMCount() const {
	return this->mMMCount;
}

bool BasePayload::deepCopyFrom(const BasePayload& pSrc) {
	for (unsigned int i = 0; i < pSrc.mExtensions.size(); i++) {
		if (pSrc.mExtensions[i]) {
			if (!mExtensions[i]) {
				ExtensionBase *ext = pSrc.mExtensions[i]->clone();
				if (ext) {
					setExtension(i, ext);
				} else {
					HV_WARN("Extension is not clonable or cloning failed")
				}
			} else {
				mExtensions[i]->copy_from(*pSrc.mExtensions[i]);
			}
		}
	}
	return true;
}

void BasePayload::updateExtensionsFrom(const BasePayload & other) {
	HV_ASSERT(mExtensions.size() <= other.mExtensions.size(),
			"Source has less extensions than destination");
	for (unsigned int i = 0; i < mExtensions.size(); i++) {
		if (other.mExtensions[i]) {
			if (mExtensions[i]) {
				mExtensions[i]->copy_from(*other.mExtensions[i]);
			}
		}
	}
}

void BasePayload::freeAllExtensions() {

}

} // namespace tlm2
} // namespace communication
} // namespace vh

