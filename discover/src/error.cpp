#include "discover.h"
#include <sstream>

std::string LibnetException::getMessage(void) {
    std::stringstream ss;
    ss << RED << _LIBNET << msg << NORMAL;
    return ss.str();
}

std::string LibpcapException::getMessage(void) {
    std::stringstream ss;
    ss << RED << _LIBPCAP << msg << NORMAL;
    return ss.str();
}