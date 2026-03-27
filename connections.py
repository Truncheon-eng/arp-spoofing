from scapy.all import *
from scapy.layers.inet import IP, TCP, UDP, ICMP
from scapy.layers.l2 import ARP
import sys

def tcp_connection(ip_addr: str, port: str):
    p = None
    try:
        packet = IP(dst=f"{ip_addr}")/TCP(dport=int(port), flags="S")
        p = sr1(packet, timeout=2, verbose=False)
    except (ValueError, TypeError):
        print(f"Value ot Type error when convering \"{port}\" to int")
    except Exception as e:
        print(f"[error]: {e} ")

    return p

def udp_connection(ip_addr: str, port: str):
    p = None
    
    try:
        packet = IP(dst=f"{ip_addr}")/UDP(dport=int(port))
        p = sr1(packet, timeout=2, verbose=False)
    except (ValueError, TypeError):
        print(f"Value ot Type error when convering \"{port}\" to int")
    except Exception as e:
        print(f"[error]: {e}")
    
    return p

def icmp_connection(ip_addr):
    p = None

    try:
        packet = IP(dst=f"{ip_addr}")/ICMP()/"My data"
        p = sr1(packet, timeout=2, verbose=False)
    except Exception as e:
        print(f"[error]: {e}")

    return p

# arp monitoring
def arp_monitor_callback(pkt):
    if ARP in pkt and pkt[ARP].op in (1,2): #who-has or is-at
        return pkt.sprintf("%ARP.hwsrc% %ARP.psrc%")

if __name__ == "__main__":
    args_ = argparse.ArgumentParser(description='.csv files format args_ and handler')
    args_.add_argument('--port', '-p', type=str, help='Interface name')
    args_.add_argument('--target', '-t', type=str, help='MAC address of A\'s host at the connection between A and B')
    
    args = args_.parse_args()

    if (args.target is None):
        print("[error]: target is None")
        sys.exit()
    
    if (args.port is None):
        print("[error]: port is None")
        sys.exit()
    
    # p = tcp_connection(args.target, args.port)
    # p = udp_connection(args.target, args.port)
    p = icmp_connection(args.target)

    if not p is None:
        p.show()
    else:
        print("[error]: connection error")
    # sniff(prn=arp_monitor_callback, filter="arp", store=0) - for ARP