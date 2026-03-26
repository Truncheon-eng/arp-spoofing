#pragma once
#include <string>
#include <map>

#define _LIBNET "[libnet] "
#define _LIBPCAP "[libpcap] "

#define NORMAL  "\033[0m"

#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define CHECK_MARK "[+] "   // Check
#define CROSS_MARK "[-] "   // Error
#define WARNING_MARK "[!] " // Warning
#define INFO_MARK "[i] "    // Info

#define MAC_LEN 6

std::map<char, char *> parse_arguments(int argc, char ** argv);

class NetException {
protected:
    std::string msg;
public:
    NetException(const std::string& _msg): msg{_msg} {};
    NetException(std::string& _msg): msg{_msg} {};
    virtual std::string getMessage() = 0;
};


class LibnetException: public NetException {
public:
    LibnetException(const std::string& _msg): NetException(msg) {};
    LibnetException(std::string& _msg): NetException(msg) {};
    std::string getMessage() override;
};

class LibpcapException: public NetException {
public:
    LibpcapException(const std::string& _msg): NetException(msg) {};
    LibpcapException(std::string& _msg): NetException(msg) {};
    std::string getMessage() override;
};
