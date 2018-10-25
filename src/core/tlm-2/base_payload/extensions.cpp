/**
 * @file extension.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Extension Mechanism
 */

#include "../../../core/tlm-2/base_payload/extensions.h"

using namespace ::hv::common;

namespace hv {
namespace communication {
namespace tlm2 {

Extensions::Extensions() :
		mMM(nullptr) {

}

Extensions::Extensions(MemoryManagementIf* MMIn) :
		mMM(MMIn) {

}

ExtensionBase* Extensions::setExtension(hvuint32_t index, ExtensionBase* ext) {
	HV_ASSERT(index < mExtensions.size(),
			"Index of extension is out of vector's scope")
	ExtensionBase* tmp = mExtensions[index];
	mExtensions[index] = ext;
	return tmp;
}

ExtensionBase*
Extensions::setAutoExtension(unsigned int index, ExtensionBase* ext) {
	sc_assert(index < mExtensions.size());
	ExtensionBase* tmp = mExtensions[index];
	mExtensions[index] = ext;
	if (!tmp)
		mExtensions.insert_in_cache(&mExtensions[index]);
	return tmp;
}

ExtensionBase* Extensions::getExtension(hvuint32_t index) const {
	HV_ASSERT(index < mExtensions.size(),
			"Index of extension is out of vector's scope")
	return mExtensions[index];
}

void Extensions::clearExtension(hvuint32_t index) {
	HV_ASSERT(index < mExtensions.size(),
			"Index of extension is out of vector's scope")
	mExtensions[index] = nullptr;
}

void Extensions::releaseExtension(hvuint32_t index) {
	HV_ASSERT(index < mExtensions.size(),
			"Index of extension is out of vector's scope")
	if (mMM) {
		mExtensions.insert_in_cache(&mExtensions[index]);
	} else {
		mExtensions[index]->free();
		mExtensions[index] = nullptr;
	}
}

void Extensions::resizeExtensions() {
	mExtensions.expand(::tlm::max_num_extensions());
}

}
}
}
