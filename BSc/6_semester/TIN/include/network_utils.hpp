/*
 * Project: TinyAd
 * Description: Utility functions for UdpConnection and networking in general
 * Author: Rafal Kulus
 * Created: 03.05.2021
 */

#ifndef NETWORK_UTILS_HPP
#define NETWORK_UTILS_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

#include <string>

#include "exceptions.hpp"

namespace TinyAd {

inline struct in6_addr strToAddr(const std::string& ip) {
    struct in6_addr ip_addr;

    if (inet_pton(AF_INET6, ip.c_str(), &ip_addr) <= 0) {
        throw SocketApiException("inet_pton");
    }

    return ip_addr;
}

inline std::string addrToStr(const struct in6_addr* ip_addr) {
    char ip[INET6_ADDRSTRLEN];

    if (inet_ntop(AF_INET6, ip_addr, ip, sizeof(ip)) == NULL) {
        throw SocketApiException("inet_ntop");
    }

    return ip;
}

inline struct sockaddr_in6 strToSockaddr(const std::string& ip, uint16_t port) {
    struct sockaddr_in6 addr = {};
    addr.sin6_family = AF_INET6;
    addr.sin6_addr = ip == "" ? in6addr_any : strToAddr(ip);
    addr.sin6_port = htons(port);

    return addr;
}

}  // namespace TinyAd

#endif