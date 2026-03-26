#include "libpcapwrap.h"


LibpcapWrapper::LibpcapWrapper(char * device, int mtu, int timeout): 
    p_ptr{pcap_open_live(device, mtu, 1, timeout, err_buf), &deleter_libpcap} {
    
    if (p_ptr.get() == nullptr)
        throw LibpcapException(std::string(err_buf));
    
    return;
}

void LibpcapWrapper::set_filter(char * ip) {
    if (ip == nullptr)
        throw LibpcapException("error in set_filter : ip == nullptr");

    char filter_string[FILTER_LEN];

    in_addr number_ip;

    if (inet_aton(ip, &number_ip) == 0)
        throw LibpcapException(std::string("error in set_filter : inet_aton(...) == 0"));

    // preparing filter string
    snprintf(
        filter_string, 
        FILTER_LEN - 1, 
        "arp and arp[6:2] = 0x%x and arp[14:4] = 0x%08x", 
        ARPOP_REPLY, 
        ntohl(number_ip.s_addr)
    );

    filter_string[FILTER_LEN - 1] = '\0';

    std::cout << YELLOW << INFO_MARK << "Debug filter: \"" << filter_string  << "\"" << NORMAL << std::endl;

    if(pcap_compile(p_ptr.get(), &filter, filter_string, 0, 0) == -1)
        throw LibpcapException(err_buf);
    
    if (pcap_setfilter(p_ptr.get(), &filter) == -1)
        throw LibpcapException(err_buf);

    std::cout << YELLOW << INFO_MARK << "Filter applied" << NORMAL << std::endl;

    return;
}

std::string LibpcapWrapper::run_loop(char * ip) {
    std::unique_ptr<char[]> s_p = std::make_unique<char[]>(MAC_LEN * 2 + MAC_LEN + 1); // array of chars for pcap_handler
    set_filter(ip);
    pcap_loop(p_ptr.get(), 1, handler, reinterpret_cast<u_char *>(s_p.get()));
    return std::string(s_p.get());
}

void LibpcapWrapper::handler(u_char * _u_ptr, const pcap_pkthdr * hdr_p, const u_char * packet) {
    char * u_ptr = reinterpret_cast<char *>(_u_ptr);
    const struct libnet_ethernet_hdr * eth_hdr = reinterpret_cast<const struct libnet_ethernet_hdr *>(packet);

    snprintf(
        u_ptr, MAC_LEN * 2 + MAC_LEN, 
        "%02x:%02x:%02x:%02x:%02x:%02x",
        eth_hdr -> ether_shost[0],
        eth_hdr -> ether_shost[1],
        eth_hdr -> ether_shost[2],
        eth_hdr -> ether_shost[3],
        eth_hdr -> ether_shost[4],
        eth_hdr -> ether_shost[5]
    );

    u_ptr[MAC_LEN * 2 + MAC_LEN] = '\0';
    return;
}