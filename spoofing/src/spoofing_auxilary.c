#include "spoofing.h"

int set_hardware_address(libnet_t * l, struct libnet_ether_addr ** pp) {
    if (l == NULL)
        return ERR;
    
    if(pp == NULL)
        return ERR;

    if ((*pp = libnet_get_hwaddr(l)) == NULL)
        return ERR;
    
    return OK;
}

void libnet_fatal(const char *failed_in, const char *errbuf) {
	printf("[%s]: Fatal Error in %s: %s\n", ERROR, failed_in, errbuf);
	exit(EXIT_FAILURE);
}


int set_broadcast_address(uint8_t ** ha_p) {
    if (ha_p == NULL)
        return ERR;
    
    *ha_p = (uint8_t *) malloc(sizeof(char) * MAC_SIZE);

    if(*ha_p == NULL)
        return ERR;

    for(size_t i = 0; i < MAC_SIZE; i++)
        (*ha_p)[i] = (uint8_t) 0xff;

    return OK;
}


