#include <libnet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>

#define ERR (-1)
#define OK 0

#define ERROR "x"
#define INFO "i"

#define MAC_SIZE 6

int set_hardware_address(libnet_t * l, struct libnet_ether_addr ** pp);

void libnet_fatal(const char *failed_in, const char *errbuf);

int set_broadcast_address(uint8_t ** ha_p); // implicit malloc