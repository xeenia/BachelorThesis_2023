#pragma once
#ifndef SCRIPTCLASS_H
#define SCRIPTCLASS_H
#include <string>
#include <iostream>
#include <map>
#include "ScriptCallable.h"
#include "ScriptInstance.h"
class ScriptFunction;
class ScriptClass : public ScriptCallable {
public:
    string name;
    ScriptClass* superclass;
    ScriptClass(string name, ScriptClass* superclass, map<string, ScriptFunction*> methods);

    ScriptFunction* findMethod(string name);
    std::string toString() override;
    virtual Token* call(Interpreter* interpreter, vector<Token*> arguments) override;

    int arity() override;
private:
    map<string, ScriptFunction*> methods;
};

#endif