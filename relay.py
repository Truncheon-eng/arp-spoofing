import sys
import argparse
from scapy.all import *
from scapy.layers.l2 import Ether

MAC_A = None 
MAC_B = None

def relay_packet(pkt):
    my_mac = get_if_hwaddr(pkt.sniffed_on)

    if pkt[Ether].src == MAC_A:
        pkt[Ether].src = my_mac
        pkt[Ether].dst = MAC_B

        sendp(pkt, verbose=0, iface = pkt.sniffed_on)
    
    elif pkt[Ether].src == MAC_B:
        pkt[Ether].src = my_mac
        pkt[Ether].dst = MAC_A

        sendp(pkt, verbose=0, iface = pkt.sniffed_on)


if __name__ == "__main__":
    args_ = argparse.ArgumentParser(description='.csv files format args_ and handler')
    args_.add_argument('--device', '-d', type=str, help='Interface name')
    args_.add_argument('--first-host', '-A', type=str, help='MAC address of A\'s host at the connection between A and B')
    args_.add_argument('--second-host', '-B', type=str, help='MAC address of B\'s host at the connection between A and B')
    
    args = args_.parse_args()

    if (args.device is None):
        print("-d/--device is None")
        sys.exit()

    if (args.first_host is None):
        print("-A/--first-host is None")
        sys.exit()

    MAC_A = args.first_host

    if (args.second_host is None):
        print("-B/--second-host is None")
        sys.exit()

    MAC_B = args.second_host

    filter_ = f"(ether src {MAC_A} or ether src {MAC_B}) and (ether dst {get_if_hwaddr(args.device)})"
    try:
        sniff(
            iface=[args.device], 
            prn=relay_packet, 
            filter=filter_
        )
    except Exception as e:
        print(e)