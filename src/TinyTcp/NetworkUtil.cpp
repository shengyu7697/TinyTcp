#include "NetworkUtil.h"

#include <unistd.h> // for close
#include <string.h> // for strncpy
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h> // for struct ifreq
#include <arpa/inet.h> // for inet_ntoa/inet_ntop
#include <netinet/in.h> // for INET_ADDRSTRLEN

std::string getEthIpAddr(const char *ifname)
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET; // get an IPv4 IP address
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1); // attached to "eth0"
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);

    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr, buf, INET_ADDRSTRLEN);

    return std::string(buf);
}
