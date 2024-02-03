/*
 * Project: TinyAd
 * Description: Tests for network utilities
 * Author: Rafal Kulus
 * Created: 04.05.2021
 */

#include "network_utils.hpp"

#include <gtest/gtest.h>

using namespace TinyAd;

TEST(NetworkUtils, strToAddr_and_addrToStr) {
    std::string ip = "ff02::1337:1";

    struct in6_addr ip_addr = strToAddr(ip);

    EXPECT_EQ(addrToStr(&ip_addr), ip);
}

TEST(NetworkUtils, strToSockaddr_standard) {
    std::string ip = "ff02::1337:1";
    int port = 25565;

    struct sockaddr_in6 addr = strToSockaddr(ip, port);

    EXPECT_EQ(addr.sin6_family, AF_INET6);
    EXPECT_EQ(addrToStr(&addr.sin6_addr), ip);
    EXPECT_EQ(ntohs(addr.sin6_port), port);
}

TEST(NetworkUtils, strToSockaddr_emptyIp_port0) {
    struct sockaddr_in6 addr = strToSockaddr("", 0);

    EXPECT_EQ(addr.sin6_family, AF_INET6);
    EXPECT_EQ(addrToStr(&addr.sin6_addr), addrToStr(&in6addr_any));
    EXPECT_EQ(addr.sin6_port, 0);
}
