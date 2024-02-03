/*
 * Project: TinyAd
 * Description: Facade for sockets; Class for simulation of datagram dropping
 * Author: Rafal Kulus
 * Created: 03.05.2021
 */

#include "udp_connection.hpp"

#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "common.hpp"
#include "exceptions.hpp"
#include "network_utils.hpp"

namespace TinyAd {

UdpConnection::UdpConnection(uint16_t port, bool reuse_port) : connected_(false) {
    sock_ = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock_ == -1) {
        throw SocketApiException("socket");
    }

    if (reuse_port) {
        int one = 1;
        if (setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one)) == -1) {
            throw SocketApiException("setsockopt SO_REUSEPORT");
        }
    }

    struct sockaddr_in6 addr = strToSockaddr("", port);

    if (bind(sock_, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        throw SocketApiException("bind");
    }
}

UdpConnection::~UdpConnection() { close(sock_); }

ssize_t UdpConnection::sendMsgTo(const std::string& buf, const struct sockaddr_in6* addr) {
    ssize_t sent = sendto(sock_, buf.c_str(), buf.size(), 0, (sockaddr*)addr, sizeof(*addr));

    if (sent == -1) {
        throw SocketApiException("sendto");
    }
    return sent;
}

ssize_t UdpConnection::sendMsg(const std::string& buf) {
    if (!connected_) {
        throw NotConnectedException();
    }

    return sendMsgTo(buf, &default_dst_);
}

std::string UdpConnection::recvMsgFrom(struct sockaddr_in6* addr, socklen_t* addr_len) {
    char buf[BUFFER_SIZE];

    if (addr_len != NULL) {
        *addr_len = sizeof(*addr);
    }

    ssize_t bytes_recved = recvfrom(sock_, buf, BUFFER_SIZE, 0, (sockaddr*)addr, addr_len);

    if (bytes_recved == -1) {
        throw SocketApiException("recvfrom");
    }
    return {buf, (unsigned long)bytes_recved};
}

std::string UdpConnection::recvMsg() { return recvMsgFrom(NULL, NULL); }

void UdpConnection::connect(struct sockaddr_in6 addr) {
    default_dst_ = addr;
    connected_ = true;
}

void UdpConnection::joinMulticastGroup(struct in6_addr ip_addr) {
    struct ipv6_mreq mreq;
    mreq.ipv6mr_interface = 0;
    mreq.ipv6mr_multiaddr = ip_addr;

    if (setsockopt(sock_, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq, sizeof(mreq))) {
        throw SocketApiException("setsockopt IPV6_JOIN_GROUP");
    }
}

int UdpConnection::getSocket() const { return sock_; }

int UdpConnection::getPort() const {
    struct sockaddr_in6 addr;
    socklen_t addr_len = sizeof(addr);

    if (getsockname(sock_, (struct sockaddr*)&addr, &addr_len) == -1) {
        throw SocketApiException("getsockname");
    }

    return ntohs(addr.sin6_port);
}

WeakUdpConnection::WeakUdpConnection(double drop_rate, uint16_t port, bool reuse_port)
    : UdpConnection(port, reuse_port), drop_rate_(drop_rate), dis_(0, 1) {}

ssize_t WeakUdpConnection::sendMsgTo(const std::string& buf, const struct sockaddr_in6* addr) {
    double luck = dis_.get();
    if (luck > drop_rate_) {
        return UdpConnection::sendMsgTo(buf, addr);
    }
    return buf.size();
}

ssize_t WeakUdpConnection::sendMsg(const std::string& buf) {
    double luck = dis_.get();
    if (luck > drop_rate_) {
        return UdpConnection::sendMsg(buf);
    }
    return buf.size();
}

std::string WeakUdpConnection::recvMsgFrom(struct sockaddr_in6* addr, socklen_t* addr_len) {
    std::string msg;

    double luck;
    bool recved_msg = false;

    while (!recved_msg) {
        luck = dis_.get();
        msg = UdpConnection::recvMsgFrom(addr, addr_len);
        if (luck > drop_rate_) {
            recved_msg = true;
        }
    }

    return msg;
}

std::string WeakUdpConnection::recvMsg() { return recvMsgFrom(NULL, NULL); }

}  // namespace TinyAd
