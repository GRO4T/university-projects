/*
 * Project: TinyAd
 * Description: Facade for sockets; Class for simulation of datagram dropping
 * Author: Rafal Kulus
 * Created: 03.05.2021
 */

#ifndef UDP_CONNECTION_HPP
#define UDP_CONNECTION_HPP

#include <netinet/in.h>
#include <stdint.h>
#include <sys/types.h>

#include <string>

#include "common.hpp"

namespace TinyAd {

class UdpConnection {
public:
    static const int BUFFER_SIZE = 1024;

    UdpConnection(uint16_t port = 0, bool reuse_port = false);
    virtual ~UdpConnection();

    virtual ssize_t sendMsgTo(const std::string& buf, const struct sockaddr_in6* addr);
    virtual ssize_t sendMsg(const std::string& buf);

    virtual std::string recvMsgFrom(struct sockaddr_in6* addr, socklen_t* addr_len);
    virtual std::string recvMsg();

    void connect(struct sockaddr_in6 addr);
    void joinMulticastGroup(struct in6_addr ip_addr);

    int getSocket() const;
    int getPort() const;

private:
    int sock_;

    bool connected_;
    struct sockaddr_in6 default_dst_;
};

class WeakUdpConnection : public UdpConnection {
public:
    WeakUdpConnection(double drop_rate, uint16_t port = 0, bool reuse_port = false);

    ssize_t sendMsgTo(const std::string& buf, const struct sockaddr_in6* addr) override;
    ssize_t sendMsg(const std::string& buf) override;

    std::string recvMsgFrom(struct sockaddr_in6* addr, socklen_t* addr_len) override;
    std::string recvMsg() override;

private:
    double drop_rate_;
    UniformRealDistribution dis_;
};

}  // namespace TinyAd

#endif