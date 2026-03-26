from scapy.all import *

MAC_A = "08:00:27:64:95:42"
MAC_B = "08:00:27:0b:ac:ad"

def relay_packet(pkt):
    # Если пакет пришел от A, меняем MAC назначения на B и отправляем
    my_mac = get_if_hwaddr(pkt.sniffed_on)
    print(my_mac)
    if pkt[Ether].src == MAC_A:
        pkt[Ether].src = my_mac
        pkt[Ether].dst = MAC_B
        del pkt[Ether].chksum
        # Удаляем src, чтобы scapy подставил наш MAC как отправителя, 
        # либо оставляем как есть для полной прозрачности
        sendp(pkt, verbose=0)
        # print("Changed from %s to %s".format(MAC_A, MAC_B))
    
    # И наоборот для ответов от B к A
    elif pkt[Ether].src == MAC_B:
        pkt[Ether].src = my_mac
        pkt[Ether].dst = MAC_A
        del pkt[Ether].chksum
        sendp(pkt, verbose=0)
        # print("Changed from %s to %s".format(MAC_B, MAC_A))

sniff(iface=["enp0s3"], prn=relay_packet, filter="ip")