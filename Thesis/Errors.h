#pragma once

#ifndef ERRORS_H
#define ERRORS_H
#include <string>
#include "RuntimeError.h"
#include "Token.h"
class Errors {
public:
    void error(int line, std::string msg);
    void error(Token* token, std::string message);
    bool getErrorType();

    void runtimeError(RuntimeError* error);
    
    bool hadRuntimeError = false;
private:
    bool hadError = false;
    void report(int line, std::string where, std::string msg);

};


#endif /* ERRORS_H */

