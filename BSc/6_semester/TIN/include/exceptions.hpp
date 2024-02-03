/*
 * Project: TinyAd
 * Description: *Some* exceptions used in the project
 * Author: Rafal Kulus
 * Created: 03.05.2021
 */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <string.h>

#include <exception>
#include <string>

namespace TinyAd {

class TinyAdException : public std::exception {
public:
    const char* what() const throw() { return msg_.c_str(); }

protected:
    std::string msg_;
};

class SocketApiException : public TinyAdException {
public:
    SocketApiException(std::string location) { msg_ = location + ": " + strerror(errno) + '\n'; }
};

class NotConnectedException : public TinyAdException {};

class FileNotFoundException : public TinyAdException {
public:
    FileNotFoundException(std::string file_path) { msg_ = "File " + file_path + " not found!\n"; }
};

class FileOpenException : public TinyAdException {
public:
    FileOpenException(std::string file_path) { msg_ = "Couldn't open " + file_path + "!\n"; }
};

class FileAllocationException : public TinyAdException {
public:
    FileAllocationException(std::string file_path, uintmax_t size) {
        msg_ = "Couldn't allocate memory for " + file_path + " - " + std::to_string(size) +
               " bytes!\n";
    }
};

}  // namespace TinyAd

#endif