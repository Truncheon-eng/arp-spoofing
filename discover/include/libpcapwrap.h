#pragma once
#include <pcap.h>
#include <libnet.h>
#include <cstring>
#include <iostream>
#include <memory>
#include "discover.h"

#define MTU 1500
#define TIMEOUT 100
#define FILTER_LEN 256

using u_pcap_t_p = std::unique_ptr<pcap_t, void (*)(pcap_t *)>;

void deleter_libpcap(pcap_t *);

class LibpcapWrapper {
public:
    LibpcapWrapper() = delete;
    LibpcapWrapper(const LibpcapWrapper&) = delete;
    LibpcapWrapper(LibpcapWrapper&) = delete;

    static void handler(u_char *, const struct pcap_pkthdr *, const u_char *); // handler to be called
    LibpcapWrapper(char * device, int mtu = MTU, int timeout = TIMEOUT);
    std::string run_loop(char * ip);
private:
    void set_filter(char * ip);

    u_pcap_t_p p_ptr;
    char err_buf[PCAP_ERRBUF_SIZE];
    struct bpf_program filter;
};