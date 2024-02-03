/*
 * Project: TinyAd
 * Description: Queue for network messages with built-in synchronization
 * Author: Rafal Kulus
 * Created: 03.06.2021
 */

#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

#include <netinet/in.h>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

namespace TinyAd {

typedef struct {
    std::string payload;
    sockaddr_in6 from;
    socklen_t from_len;
} MsgQueueEl;

class MessageQueue {
public:
    MessageQueue(uint max_size);

    bool push(const std::string& msg, const sockaddr_in6& from = {}, const socklen_t& from_len = 0);
    MsgQueueEl pop();

private:
    std::queue<MsgQueueEl> queue_;
    uint max_size_;

    std::mutex m_;
    std::condition_variable cond_reader_;
};

}  // namespace TinyAd

#endif