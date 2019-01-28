/**
 * @file extension.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM Extension Mechanism
 */

#ifndef HV_TLM_EXTENSIONS_H_
#define HV_TLM_EXTENSIONS_H_

#include <cstdlib>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <cstring>
#include <HVCommon>
#include <tlm>



namespace hv {
namespace communication {
namespace tlm2 {

/**
 * Forward declaration of MemoryManagementIf
 */
class MemoryManagementIf;

/**
 * HV extension base is TLM extension base
 * because TLM extension system is great
 */
typedef ::tlm::tlm_extension_base ExtensionBase;

#ifndef HV_CPLUSPLUS_NO_ALIAS_TEMPLATE
template<typename T> using Extension = ::tlm::tlm_extension<T>;
#else
template<typename T> class Extension : public ::tlm::tlm_extension<T> {
	virtual ::tlm::tlm_extension_base* clone() const;
	virtual void copy_from(::tlm::tlm_extension_base const &ext);
	virtual ~Extension() {}
};
#endif

/**
 * Dynamic extensions mechanism class
 * This is based on a sub-part of TLM generic payload (the one managing extensions)
 * This class is then inherited in HV base_payload
 */
class Extensions {
public:
	Extensions();

	Extensions(MemoryManagementIf* MMIn);

	virtual ~Extensions() {
	}

	/**
	 * Insert extension to vector
	 * @param ext Extension pointer
	 * @return Previous value
	 */
	template<typename T> T* setExtension(T* ext) {
		return static_cast<T*>(this->setExtension(T::ID, ext));
	}

	/**
	 * Insert extension to vector with manual index
	 * This has to be used to register several identical extensions
	 * @param index Index where to insert extension
	 * @param ext Extension pointer
	 * @return Old extension value in this place
	 */
	ExtensionBase* setExtension(::hv::common::hvuint32_t index, ExtensionBase* ext);

	/**
	 * Insert extension to vector (memory-managed version)
	 * @param ext Extension pointer
	 * @return Previous extension value
	 */
	template<typename T> T* setAutoExtension(T* ext) {
		return static_cast<T*>(setAutoExtension(T::ID, ext));
	}

	ExtensionBase* setAutoExtension(unsigned int index, ExtensionBase* ext);

	/**
	 * Check for extension
	 * extension will be set to nullptr if extension is not found
	 * @param ext Extension address
	 */
	template<typename T> void getExtension(T*& ext) const {
		ext = this->getExtension<T>();
	}

	/**
	 * Check for extension
	 * returns nullptr if extension is not found
	 */
	template<typename T> T** getExtension() const {
		return static_cast<T*>(this->getExtension(T::ID));
	}

	/**
	 * Check for extension with manual index
	 * @param index Index of extension
	 * @return Pointer to extension
	 */
	ExtensionBase* getExtension(::hv::common::hvuint32_t index) const;

	/**
	 * Removes extension from the vector but does not free anything
	 * @param ext Extension address for template resolution
	 */
	template<typename T> void clearExtension(const T* ext) {
		this->clearExtension<T>();
	}

	/**
	 * Removes extension from the vector but does not free anything
	 */
	template<typename T> void clearExtension() {
		this->clearExtension(T::ID);
	}

private:
	/**
	 * Removes extension from the vector but does not free anything - manual index
	 */
	void clearExtension(::hv::common::hvuint32_t index);

public:
	/**
	 * Removes extension from the vector and frees it
	 * @param ext Extension address for template resolution
	 */
	template<typename T> void releaseExtension(const T* ext) {
		this->releaseExtension<T>();
	}

	/**
	 * Removes extension from the vector and frees it
	 */
	template<typename T> void releaseExtension() {
		this->releaseExtension(T::ID);
	}

	void resizeExtensions();

private:
	/**
	 * Removes extension from the vector and frees it - manual index
	 */
	void releaseExtension(::hv::common::hvuint32_t index);

protected:

	/**
	 * Pointer to memory management interface
	 */
	MemoryManagementIf* mMM;

	/**
	 * Vector of extensions
	 */
	::tlm::tlm_array<ExtensionBase*> mExtensions;

};

}
}
}

#endif
