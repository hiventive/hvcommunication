/**
 * @file base_payload.h
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

#ifndef HV_TLM_2_PAYLOAD_BASE_PAYLOAD_H
#define HV_TLM_2_PAYLOAD_BASE_PAYLOAD_H

#include <typeinfo>
#include <vector>
#include <hv/common.h>

#include "../../../core/tlm-2/base_payload/extensions.h"

namespace hv {
namespace communication {
namespace tlm2 {

/**
 * Base payload forward declaration for memory-mapping interface
 */
class BasePayload;

/**
 * Memory-management interface
 */
class MemoryManagementIf {
public:
	/**
	 * This method must be implemented to free the original payload
	 * A simple call to delete should be enough
	 * @param Pointer to the original payload
	 */
	virtual void free(BasePayload*) = 0;

	/**
	 * Destructor
	 */
	virtual ~MemoryManagementIf() {
	}
};

class BasePayload: public Extensions {
public:
	/**
	 * Generic constructor
	 */
	BasePayload();

	/**
	 * Constructor taking a memory management object address
	 * with this payload and this payload is the originator
	 */
	explicit BasePayload(MemoryManagementIf* MMIn);

	/**
	 * Destructor
	 */
	virtual ~BasePayload();

	/**
	 * Notify current object of the creation of a new memory-mapped object
	 */
	void acquire();

	/**
	 * Notify current object of the release of a memory-mapped object
	 */
	void release();

	/**
	 * Get number of memory-managed objects related to this one
	 * @return value of mMMCount
	 */
	std::size_t getMMCount() const;

	/**
	 * Link this object to a memory management object
	 * @param MMIn Pointer to memory management interface
	 */
	void setMM(MemoryManagementIf* MMIn);

	/**
	 * Check if this object has external memory management
	 * @return true if it does, false otherwise
	 */
	bool hasMM() const;

	/**
	 * Deep copy from other payload
	 *
	 * This function handles extensions copy
	 * Therefore, it should be called from any implementation of
	 * BasePayload allowing extensions
	 * @param pSrc Source for copy
	 * @return true if success
	 */
	bool deepCopyFrom(const BasePayload& pSrc);

    void updateExtensionsFrom(const BasePayload & other);

    void freeAllExtensions();

    virtual std::string getResponseString() const = 0;

private:
	// Deleting default copy constructor and assignment operator
	// FIXME when old VS versions won't be supported use delete keyword
	// for more graceful error messages when attempted to be used
	BasePayload(const BasePayload& bp);
	BasePayload& operator =(const BasePayload &bp);

public:
	// TODO: add possibility to deep-copy BasePayload ? I'll do it when needed

private:
	/**
	 * Counter giving the number of objects linked to this by the same memory-management object
	 */
	std::size_t mMMCount;
public:

};

} // namespace tlm2
} // namespace communication
} // namespace hv

#endif // HV_TLM_2_PAYLOAD_BASE_PAYLOAD_H
