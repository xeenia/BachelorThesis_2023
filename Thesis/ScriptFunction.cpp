#include "ScriptFunction.h"
#include "Environment.h"

ReturnV::ReturnV(Token* value) : std::runtime_error("Return"), value(value) {
}

ScriptFunction::ScriptFunction(FunctionStmt* declaration, Environment* closure, bool isInitializer) :
    declaration(declaration), closure(closure), isInitializer(isInitializer) {}

Token* ScriptFunction::call(Interpreter* interpreter, const std::vector<Token*> arguments) {
    Environment* environment =  new Environment(closure);
    
    for (int i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i]->lexeme, arguments[i]);
    }

    try {
        interpreter->executeBlock(declaration->body, environment);
    }
    catch (ReturnV* returnValue) {
        if (isInitializer) return closure->getAt(0, "this");
        return returnValue->value;
    }

    if (isInitializer) return closure->getAt(0, "this");

    return nullptr;
}
ScriptFunction* ScriptFunction::bind(ScriptInstance* instance) {
    Environment* environment = new Environment(closure);
    Token* token = new Token(TokenType::INSTANCE, instance->toString(), new LiteralU(instance), ValueType::NONVALUE, 0);
    environment->define("this", token);
    return new ScriptFunction(declaration, environment, isInitializer);
}

int ScriptFunction::arity() {
    return declaration->params.size();
}

std::string ScriptFunction::toString() {
    return "<fn " + declaration->name->lexeme + ">";
}
