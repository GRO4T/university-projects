/*
 * Project: TinyAd
 * Description: Message deserializing methods
 * Author: Damian Kolaska
 * Created: 30.04.2021
 */

#ifndef MESSAGE_DESERIALIZER_HPP
#define MESSAGE_DESERIALIZER_HPP

#include <exception>
#include <memory>

#include "exceptions.hpp"
#include "message.hpp"

namespace TinyAd {

using StringConstIt = std::string::const_iterator;

class NoMessageFound : public TinyAdException {
public:
    NoMessageFound() { msg_ = "No message found"; }
};

std::unique_ptr<Message> deserialize(StringConstIt& begin, StringConstIt end);

}  // namespace TinyAd

#endif