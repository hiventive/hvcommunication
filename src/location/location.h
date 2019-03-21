/**
 * @file location.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive IP Location Management
 */

#ifndef HV_CORE_TLM_2_SOCKETS_LOCATION_H_
#define HV_CORE_TLM_2_SOCKETS_LOCATION_H_

#include <hv/configuration.h>
#include <hv/common.h>

namespace hv {
namespace communication {
namespace tlm2 {

/**
 * Location class
 *
 * Requirements:
 * - SIZE_TYPE must be convertible to ADDR_TYPE
 * - Operators +/- must exist for ADDR_TYPE
 * - unsigned int (native type) must be convertible to ADDR_TYPE
 * - For higher compatibility with other Location classes, make sure
 *   ADDR_TYPE and SIZE_TYPE are operable with most current native types
 */
template<typename ADDR_TYPE = ::hv::common::hvaddr_t,
		typename SIZE_TYPE = std::size_t> class Location {
	template<typename ADDR_TYPE2, typename SIZE_TYPE2> friend class Location;

public:
	typedef ADDR_TYPE address_type;
	typedef SIZE_TYPE size_type;

	Location() :
			mAddress(0u), mSize(0u), mHasSize(true) {

	}

	explicit Location(const address_type &address_, const size_type& size_ =
			static_cast<size_type>(0u)) :
			mAddress(address_), mSize(size_), mHasSize(!!size_) {
	}

	//** "Copy" constructors **//
	template<typename ADDR_TYPE2, typename SIZE_TYPE2> explicit Location(
			const Location<ADDR_TYPE2, SIZE_TYPE2> src) :
			mAddress(static_cast<address_type>(src.getAddress())), mSize(
					src.mHasSize ?
							static_cast<size_type>(src.mSize) :
							static_cast<size_type>(0u)), mHasSize(src.mHasSize) {

	}

	Location(const Location &src) :
			mAddress(src.getAddress()), mSize(
					src.mHasSize ? src.mSize : static_cast<size_type>(0u)), mHasSize(
					src.mHasSize) {

	}

	virtual ~Location() {
	}

	//** Accessors **//
	/**
	 * Get address
	 * @return Current location's address
	 */
	address_type getAddress() const {
		return mAddress;
	}

	/**
	 * Get size
	 * @return Current location's size
	 * Fails if current location has no size
	 */
	size_type getSize() const {
		HV_ASSERT(mHasSize, "Impossible to get location size (not applicable)");
		return mSize;
	}

	/**
	 * Check if current location has size
	 * @return True if it has a size, false otherwise
	 */
	bool hasSize() const {
		return mHasSize;
	}

	//** Modifiers **//
	/**
	 * Set location's address
	 * @param addr New address
	 */
	void setAddress(const address_type &addr) {
		mAddress = addr;
	}

	/**
	 * Set location's size
	 * @param size New location's size
	 */
	void setSize(const size_type &size) {
		mSize = size;
		if (mSize != 0u) {
			mHasSize = true;
		} else {
			mHasSize = false;
		}
	}

	//** Ordering and belonging **//
	/**
	 * Inferiority comparison
	 * @param other Other location
	 * @return True if this location is stricly inferior to the other one (inferior and not recovering), false otherwise
	 */
	template<typename ADDR_TYPE2, typename SIZE_TYPE2> bool operator <(
			const Location<ADDR_TYPE2, SIZE_TYPE2>& other) const {
		if (sizeof(address_type) <= sizeof(ADDR_TYPE2)) {
			Location<ADDR_TYPE2, SIZE_TYPE2> locTmp(*this);
			return locTmp < other;
		}

		Location locTmp2(static_cast<address_type>(other.getAddress()),
				other.hasSize() ? other.getSize() : static_cast<size_type>(0u));
		return this->operator <(locTmp2);

	}

	template<typename ADDR_TYPE2> bool containsAddress(
			const ADDR_TYPE2 &addr) const {
		if (sizeof(ADDR_TYPE) <= sizeof(ADDR_TYPE2)) {
			if (!mHasSize) {
				return addr == static_cast<ADDR_TYPE2>(mAddress);
			}
			return ((addr >= static_cast<ADDR_TYPE2>(mAddress))
					&& (addr <= static_cast<ADDR_TYPE2>(this->getEndAddress())));
		} else {
			if (!mHasSize) {
				return addr == mAddress;
			}
			return ((addr >= mAddress) && (addr <= this->getEndAddress()));
		}
	}

	bool containsAddress(const address_type &addr) const {
		if (!mHasSize) {
			return addr == mAddress;
		}
		return ((addr >= mAddress) && (addr <= this->getEndAddress()));
	}

	/**
	 * Inferiority comparison
	 * @param other Other location
	 * @return True if this location is stricly inferior to the other one (inferior and not recovering), false otherwise
	 */
	bool operator <(const Location& other) {
		return this->getEndAddress() < other.mAddress;
	}

	/**
	 * Equality operator
	 * @param other Other location
	 * @return True if locations RECOVER (not necessarily strictly the same), false otherwise
	 */
	template<typename ADDR_TYPE2, typename SIZE_TYPE2> bool operator ==(
			const Location<ADDR_TYPE2, SIZE_TYPE2> &other) const {
		return this->containsAddress(other.getAddress())
				|| this->containsAddress(other.getEndAddress());
	}

	/**
	 * Equality operator
	 * @param other Other location
	 * @return True if locations RECOVER (not necessarily strictly the same), false otherwise
	 */
	bool operator ==(const Location& other) const {
		return this->containsAddress(other.getAddress())
				|| this->containsAddress(other.getEndAddress());
	}

//** Helpers **//
	address_type getEndAddress() const {
		return mHasSize ?
				mAddress + static_cast<address_type>(mSize)
						- static_cast<address_type>(1u) :
				mAddress;
	}

	friend std::ostream& operator <<(std::ostream &strm, const Location &loc) {
		if (loc.hasSize()) {
			return strm << std::hex << "(0x" << loc.getAddress() << ", 0x"
					<< loc.getSize() << ")" << std::dec;
		}
		return strm << std::hex << "0x" << loc.getAddress() << std::dec;
	}

protected:
	address_type mAddress;
	size_type mSize;
	bool mHasSize;
};

}
}
}

// Implementation of cci_value converter from/to Location
namespace cci {

template<typename ADDR_TYPE, typename SIZE_TYPE> struct cci_value_converter<
		::hv::communication::tlm2::Location<ADDR_TYPE, SIZE_TYPE>> {
	typedef ::hv::communication::tlm2::Location<ADDR_TYPE, SIZE_TYPE> type;

	static bool pack(cci_value::reference dst, type const& src) {
		cci_value_map_ref mref(dst.set_map());
		mref.push_entry("address", src.getAddress());
		if (src.hasSize()) {
			mref.push_entry("size", src.getSize());
		}
		return true;
	}

	static bool unpack(type & dst, cci_value::const_reference src) {
		if (!src.is_map())
			return false;

		cci_value::const_map_reference m = src.get_map();
		typename type::address_type addr;
		if (m.has_entry("address") && m.at("address").try_get(addr)) {
			typename type::size_type size;
			if (m.has_entry("size")) {
				if (!(m.at("size").try_get(size))) {
					return false;
				}
			} else {
				size = static_cast<typename type::size_type>(0u);
			}
			dst.setAddress(addr);
			dst.setSize(size);
		}
		return true;
	}
};

}

#endif /* HV_CORE_TLM_2_SOCKETS_LOCATION_H_ */
