#include "ScriptInstance.h"
#include "Token.h"
#include "ScriptFunction.h"
ScriptInstance::ScriptInstance(ScriptClass* klass) : klass(klass) {}

Token* ScriptInstance::get(Token* name) {
    if (fields.find(name->lexeme) != fields.end()) {
        return fields[name->lexeme];
    }

    ScriptFunction* method = klass->findMethod(name->lexeme);
    if (method != nullptr) {
        ScriptFunction* l = method->bind(this);
        return new Token(TokenType::CALLABLE, l->toString(), new LiteralU(l), ValueType::NONVALUE, 0);
    }
    throw new RuntimeError(name, "Undefined property '" + name->lexeme + "'.");
}

void ScriptInstance::set(Token* name, Token* value) {
    fields[name->lexeme] = value;
}
string ScriptInstance::toString() {
    return klass->name + " instance";
}