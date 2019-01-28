/**
 * @file base_socket.cpp
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM base socket for Hiventive extra features
 */

#include "base_socket.h"

namespace hv {
namespace communication {
namespace tlm2 {

IOElement::IOElement(const ::std::string &name_, const IOMode &mode_) : mName(name_), mMode(mode_) {
}

IOElement::IOElement(const IOElement &elem_) : mName(elem_.getName()), mMode(elem_.getMode()) {
}

IOElement::~IOElement() {
}

::std::string IOElement::getName() const {
    return mName;
}

IOMode IOElement::getMode() const {
    return mMode;
}

BaseIO::BaseIO() {
}

BaseIO::BaseIO(const BaseIO& other_) {
    this->mIOElements = other_.mIOElements;
}

BaseIO::~BaseIO() {
}

::std::size_t BaseIO::getNIOElements() const {
    return mIOElements.size();
}

IOElement BaseIO::getIOElement(const ::std::size_t& ind) const {
    if(ind >= getNIOElements()) {
        HV_ERR("index is out of scope");
    }
    return mIOElements[ind];
}

::std::string BaseIO::getIOElementName(const ::std::size_t& ind) const {
    return getIOElement(ind).getName();
}

IOMode BaseIO::getIOElementMode(const ::std::size_t& ind) const {
    return getIOElement(ind).getMode();
}

void BaseIO::addIOElement(const IOElement& mIOElement) {
    for(auto &it : mIOElements) {
        if(it.getName().compare(mIOElement.getName()) == 0) {
            HV_ERR("IO element name already exists");
        }
    }
    mIOElements.push_back(mIOElement);
}

void BaseIO::addIOElement(const ::std::string &name_, const IOMode &mode_) {
    this->addIOElement(IOElement(name_, mode_));
}

bool BaseIO::hasElement(const ::std::string& name_) const {
    for(auto &it : mIOElements) {
        if(it.getName() == name_) { 
            return true;
        }
    }
    return false;
}

BaseSocket::BaseSocket() {
}

BaseSocket::~BaseSocket() {
}

BaseIO BaseSocket::getBaseIO() const {
    return this->mBaseIO;
}

::std::map<::std::string, ::std::string> BaseSocket::getBindingRules() const {
    return this->mBindingRules;
}

void BaseSocket::addBaseIO(const BaseIO& baseIO_) {
    this->mBaseIO = baseIO_;
}

void BaseSocket::addBindingRule(const ::std::string& elem1_, const ::std::string& elem2_) {
    if(!mBaseIO.hasElement(elem1_)) {
        HV_ERR("First element does not exist in IO elements");
    }

    mBindingRules[elem1_] = elem2_;
}

} // namespace tlm2
} // namespace communication
} // namespace hv

