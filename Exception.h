#pragma once

#include <stdexcept>
#include <utility>

class Exception : public std::exception {
private:
    std::string msg_error;
public:
    explicit Exception(std::string msg_error) : msg_error(std::move(msg_error)) {}
    ~Exception() override = default;

    const char* what() {
        return msg_error.c_str();
    }
};