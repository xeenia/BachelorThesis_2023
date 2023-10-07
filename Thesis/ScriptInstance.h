#pragma once
#ifndef SCRIPTINSTANCE_H
#define SCRIPTINSTANCE_H
#include <string>
#include <iostream>
#include <map>
#include "ScriptClass.h"
#include "ScriptInstance.h"
class ScriptClass;
class ScriptInstance {
public:

    ScriptInstance(ScriptClass* klass);

    Token* get(Token* name);
    void set(Token* name, Token* value);
    std::string toString();
private:
    ScriptClass* klass;
    std::map<string, Token*> fields; 
};

#endif