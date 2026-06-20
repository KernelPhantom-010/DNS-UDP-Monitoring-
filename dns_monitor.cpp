#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <signal.h>
#include <netinet/udp.h>
#include <thread>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <errno.h>
#include <linux/if_packet.h>
#include <net/if.h>

void ctrlCatch(int s)
{
    printf("\n=== Monitoring ended ===\n");
    exit(0);
}

void monitoring(int sd)
{
    sockaddr_ll myStrc;
    memset(&myStrc, 0, sizeof(myStrc));

    myStrc.sll_family = AF_PACKET;
    myStrc.sll_protocol = htons(ETH_P_ALL);
    myStrc.sll_ifindex = if_nametoindex("wlx202351d079cd");

    if (bind(sd, (sockaddr*)&myStrc, sizeof(myStrc)) == 0)
    {
        std::cout << "[+] Bound to interface\n";
    }
    else
    {
        std::cout << "[+] Failed to bind\n[ERR:" << errno << "]\n";
        return;
    }

    std::cout << "[+] Listening..\n";

    while (1)
    {
        char msgBuffer[65536];

        int bytesReceived = recvfrom(
            sd,
            msgBuffer,
            sizeof(msgBuffer),
            0,
            nullptr,
            nullptr);

        if (bytesReceived <= 0)
            continue;

        struct ethhdr* eth = (struct ethhdr*)msgBuffer;

        if (ntohs(eth->h_proto) != ETH_P_IP)
            continue;

        struct iphdr* ip_hdr =
            (struct iphdr*)(msgBuffer + sizeof(struct ethhdr));

        if (ip_hdr->protocol != IPPROTO_UDP)
            continue;

        struct udphdr* udp_hdr =
            (struct udphdr*)(msgBuffer +
                             sizeof(struct ethhdr) +
                             (ip_hdr->ihl * 4));

        struct in_addr srcAddr;
        struct in_addr dstAddr;

        srcAddr.s_addr = ip_hdr->saddr;
        dstAddr.s_addr = ip_hdr->daddr;

        uint16_t srcPort = ntohs(udp_hdr->source);
        uint16_t dstPort = ntohs(udp_hdr->dest);

        std::cout << "\n[UDP Packet]\n";
        std::cout << "Source IP   : " << inet_ntoa(srcAddr) << '\n';
        std::cout << "Dest IP     : " << inet_ntoa(dstAddr) << '\n';
        std::cout << "Source Port : " << srcPort << '\n';
        std::cout << "Dest Port   : " << dstPort << '\n';

        if (srcPort == 53 || dstPort == 53)
        {
            std::cout << "[DNS Traffic Detected]\n";
        }
    }
}

int main()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = ctrlCatch;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    const char* wlcmTxt = "### DNS-Monitoring ###\n\n";

    for (int x = 0; x < strlen(wlcmTxt); ++x)
    {
        printf("%c", wlcmTxt[x]);
        fflush(stdout);
        usleep(10000);
    }

    char c[4096];

    printf("[+] Press enter to continue..\n");
    fgets(c, sizeof(c), stdin);

    int socketDesc =
        socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (socketDesc == -1)
    {
        printf("[+] socket-creation failed -> %d\n", errno);
        exit(0);
    }

    printf("[+] socket created\n");

    monitoring(socketDesc);

    close(socketDesc);

    return 0;
}