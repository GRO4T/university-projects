/*
 * Project: TinyAd
 * Description: Queue for network messages with built-in synchronization
 * Author: Rafal Kulus
 * Created: 03.06.2021
 */

#include "message_queue.hpp"

namespace TinyAd {

MessageQueue::MessageQueue(uint max_size) : max_size_(max_size) {}

bool MessageQueue::push(const std::string& msg, const sockaddr_in6& from,
                        const socklen_t& from_len) {
    MsgQueueEl mqe = {msg, from, from_len};

    std::lock_guard lock(m_);
    if (queue_.size() < max_size_) {
        queue_.push(mqe);
        if (queue_.size() == 1) {
            cond_reader_.notify_one();
        }
        return true;
    }
    return false;
}

MsgQueueEl MessageQueue::pop() {
    std::unique_lock lock(m_);
    if (queue_.empty()) {
        cond_reader_.wait(lock, [this] { return !queue_.empty(); });
    }
    MsgQueueEl mqe = queue_.front();
    queue_.pop();
    return mqe;
}

}  // namespace TinyAd
