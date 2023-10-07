#ifndef SCRIPTFUNCTION_H
#define SCRIPTFUNCTION_H 

#include "ScriptCallable.h"
#include "Interpreter.h"
#include "Environment.h"
class ReturnV : public std::runtime_error {
public:
    Token* value;

    ReturnV(Token* value);
};
class ScriptFunction : public ScriptCallable {
private:
    FunctionStmt* declaration;
    Environment* closure;
    bool isInitializer;

public:
    ScriptFunction* bind(ScriptInstance* instance);
    Token* call(Interpreter* interpreter, const std::vector<Token*> arguments) override;
    int arity() override;
    std::string toString() override;
    ScriptFunction(FunctionStmt* declaration, Environment* closure, bool isInitializer);
};

#endif



