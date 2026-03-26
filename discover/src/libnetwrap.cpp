#include "libnetwrap.h"

// default constructor
LibnetWrapper::LibnetWrapper(const char * device):
    l_ptr(libnet_init(LIBNET_LINK, device, err_buf), &deleter_libnet),
    arp_tag(LIBNET_PTAG_INITIALIZER), eth_tag(LIBNET_PTAG_INITIALIZER) {

    if (l_ptr.get() == nullptr)
        throw LibnetException(std::string("libnet_init error -> ") + std::string(err_buf));
    
    // getting source port address
    uint32_t spa = libnet_get_ipaddr4(l_ptr.get());
    if (spa == static_cast<uint32_t>(-1)) {
        libnet_geterror(l_ptr.get());
        throw LibnetException(std::string(err_buf));
    }

    struct libnet_ether_addr * ptr = nullptr;

    // getting mac address
    if ((ptr = libnet_get_hwaddr(l_ptr.get())) == nullptr) {
        libnet_geterror(l_ptr.get());
        throw LibnetException(std::string(err_buf));
    }
    
    // copy mac address to local private field
    std::memcpy(&mac, &(ptr -> ether_addr_octet[0]), MAC_LEN); // copy of MAC address
    return;
}

void LibnetWrapper::make_arp_request(char * ip) {
    if (ip == nullptr)
        throw LibnetException(std::string("ip == nullptr when call \"make_arp_request\""));
    
    uint32_t tpa = libnet_name2addr4(l_ptr.get(), ip, LIBNET_DONT_RESOLVE);
    
    if(tpa == static_cast<uint32_t>(-1))
        throw LibnetException(std::string("libnet_name2addr4 error -> ") + std::string(err_buf));
    
    uint8_t tha[MAC_LEN] {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    arp_tag = libnet_autobuild_arp(
        ARPOP_REQUEST,
        reinterpret_cast<uint8_t *>(&mac),
        reinterpret_cast<uint8_t *>(&spa),
        reinterpret_cast<uint8_t *>(&tha),
        reinterpret_cast<uint8_t *>(&tpa),
        l_ptr.get()
    );

    if(arp_tag == -1)
        throw LibnetException(std::string("libnet_autobuild_arp error -> ") + std::string(err_buf));

    eth_tag = libnet_autobuild_ethernet(
        reinterpret_cast<uint8_t *>(&tha),
        ETHERTYPE_ARP,
        l_ptr.get()      
    );

    if (eth_tag == -1)
        throw LibnetException(std::string("libnet_autobuild_ethernet error -> ") + std::string(err_buf));

    int bytes_written = libnet_write(l_ptr.get());

    if (bytes_written == -1)
        throw LibnetException(std::string(err_buf));

    std::cout << YELLOW << INFO_MARK << "bytes written = " << bytes_written << NORMAL << std::endl;
    return;
}