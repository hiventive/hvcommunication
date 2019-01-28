/**
 * @file base_socket.h
 * @author Benjamin Barrois <benjamin.barrois@hiventive.com>
 * @date Aug, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive TLM base socket for Hiventive extra features
 */

#ifndef HV_CORE_TLM_2_SOCKETS_BASE_SOCKET_H_
#define HV_CORE_TLM_2_SOCKETS_BASE_SOCKET_H_

#include<HVCommon>
#include<vector>

namespace hv {
namespace communication {
namespace tlm2 {

enum IOMode {
	IN_MODE,
	OUT_MODE,
    INOUT_MODE
};

class IOElement {
public:
    IOElement(const ::std::string &name_, const IOMode &mode_);
    IOElement(const IOElement &elem_);

    ~IOElement();

    ::std::string getName() const;
    IOMode getMode() const;
protected:
    ::std::string mName;
    IOMode mMode;
};

class BaseIO {
public:
    BaseIO();
    BaseIO(const BaseIO& other_);
    ~BaseIO();

    ::std::size_t getNIOElements() const;
    IOElement getIOElement(const ::std::size_t& ind) const;
    ::std::string getIOElementName(const ::std::size_t& ind) const;
    IOMode getIOElementMode(const ::std::size_t& ind) const;
    bool hasElement(const ::std::string& name_) const;

    void addIOElement(const IOElement& mIOElement);
    void addIOElement(const ::std::string &name_, const IOMode &mode_);

protected:
    ::std::vector<IOElement> mIOElements;
};

class BaseSocket {
public:
    BaseSocket();
    ~BaseSocket();

    BaseIO getBaseIO() const;
    ::std::map<::std::string, ::std::string> getBindingRules() const;

    void addBaseIO(const BaseIO& baseIO_);
    void addBindingRule(const ::std::string& elem1_, const ::std::string& elem2_);
protected:
    BaseIO mBaseIO;
    ::std::map<::std::string, ::std::string> mBindingRules;
};

} // namespace tlm2
} // namespace communication
} // namespace hv

#endif
