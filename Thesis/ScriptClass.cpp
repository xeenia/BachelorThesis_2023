#include "ScriptClass.h"
#include "TokenType.h"
#include "Token.h"
#include "ScriptFunction.h"
#include "ScriptInstance.h"
ScriptClass::ScriptClass(string name, ScriptClass* superclass, map<string, ScriptFunction*> methods) : name(name), superclass(superclass), methods(methods) {}

ScriptFunction* ScriptClass::findMethod(string name) {
    if (methods.find(name) != methods.end()) {
        return methods[name];
    }
    if (superclass != nullptr) {
        return superclass->findMethod(name);
    }
    return nullptr;
}
std::string ScriptClass::toString() {
    return name;
}

Token* ScriptClass::call(Interpreter* interpreter, vector<Token*> arguments) {
    ScriptInstance* instance = new ScriptInstance(this);
    ScriptFunction* initializer = findMethod("init");
    if (initializer != nullptr) {
        initializer->bind(instance)->call(interpreter, arguments);
    }
    Token* token = new Token(TokenType::INSTANCE, instance->toString(), new LiteralU(instance), ValueType::NONVALUE, 0);
    return token;
}

int ScriptClass::arity() {
    ScriptFunction* initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}