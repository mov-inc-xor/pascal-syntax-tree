#pragma once

#include <string>

#include "Exception.h"

class SyntaxError : public Exception {
public:
    explicit SyntaxError(const std::string& msg) : Exception("Синтаксическая ошибка: " + msg) {}
};
