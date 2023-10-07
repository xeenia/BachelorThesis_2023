#pragma once


#ifndef RUNTIMEERROR_H
#define RUNTIMEERROR_H

#include <stdexcept>
#include "Token.h"
#include <string>

class RuntimeError : public std::runtime_error {
public:
    Token* token;
    const string message;
    RuntimeError(Token* token, const std::string& message);
    string getMessage();
};

#endif  // RUNTIMEERROR_H