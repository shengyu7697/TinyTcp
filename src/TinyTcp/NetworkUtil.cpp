#include "NetworkUtil.h"

#include <unistd.h> // for close
#include <string.h> // for strncpy
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h> // for struct ifreq
#include <arpa/inet.h> // for inet_ntoa

std::string getEthIpAddr(const char *ifname)
{
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    // get an IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;

    // I want IP address attached to "eth0"
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    return std::string("");
}
