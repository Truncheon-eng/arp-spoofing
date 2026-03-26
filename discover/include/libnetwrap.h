#pragma once
#include <libnet.h>
#include <cstring>
#include <iostream>
#include <memory>
#include "discover.h"

using u_libnet_t_p = std::unique_ptr<libnet_t, void (*) (libnet_t *)>;

void deleter_libnet(libnet_t * p);

class LibnetWrapper {
public:
    LibnetWrapper(const char * device); // +
    void make_arp_request(char * ip); // +
    
    LibnetWrapper() = delete;
    LibnetWrapper(const LibnetWrapper&) = delete;
    LibnetWrapper(LibnetWrapper&) = delete;
private:
    u_libnet_t_p l_ptr;
    char err_buf[LIBNET_ERRBUF_SIZE];
    uint32_t spa;
    uint8_t mac[MAC_LEN];
    libnet_ptag_t arp_tag;
    libnet_ptag_t eth_tag;
};