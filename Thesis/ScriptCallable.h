#pragma once 

#ifndef SCRIPTCALLABLE_H
#define SCRIPTCALLABLE_H

#include <list>
#include <string>
#include <chrono>

using namespace std;
class Token;
class Interpreter;

class ScriptCallable {
public:
    virtual Token* call(Interpreter* interpreter, const std::vector<Token*> arguments) = 0;
    virtual int arity() = 0;
    virtual string toString() = 0;
};


#endif
