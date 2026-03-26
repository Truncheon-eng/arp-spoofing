#include "libnetwrap.h"
#include "libpcapwrap.h"

#include <map>

void deleter_libnet(libnet_t * p) {
    if (p) {
        libnet_destroy(p); 
        std::cout << YELLOW << INFO_MARK << "libnet_destroy and deleter_libnet called" << NORMAL << std::endl;
    } else {
        std::cout << YELLOW << INFO_MARK << "deleter_libnet called" << NORMAL << std::endl;
    }
    return;
}

void deleter_libpcap(pcap_t * p) {
    if (p) {
        pcap_close(p);
        std::cout << YELLOW << INFO_MARK << "pcap_close and deleter_libpcap called" << NORMAL << std::endl;
    } else {
        std::cout << YELLOW << INFO_MARK << "deleter_libpcap called" << NORMAL << std::endl;
    }
    return;
}


std::map<char, char *> parse_arguments(int argc, char ** argv) {
    int param = -1;
    std::map<char, char *> param_value;
    while ((param = getopt(argc, argv, "t:d:")) != -1) {
        switch (param)
        {
        case 'd':
        case 't':
        {
            if (param_value.count(param))
                continue;
            param_value[param] = optarg;
            break;
        }
        default:
            break;
        }
    }
    return param_value;
}