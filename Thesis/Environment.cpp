#include "Environment.h"
#include "RuntimeError.h"

Environment::Environment() {
    enclosing = nullptr;
}

Environment::Environment(Environment* enclosing) {
    this->enclosing = enclosing;
}

void Environment::define(string name, Token* value) {
	values[name] = value;
}


Token* Environment::getAt(int distance, string name) {
    Environment* rn = ancestor(distance);
    return rn->values[name];
}

Environment* Environment::ancestor(int distance) {
    Environment* environment = this;
    for (int i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }

    return environment;
}

void Environment::assignAt(int distance, Token* name, Token* value) {
    ancestor(distance)->values[name->lexeme] = value;
}

Token* Environment::get(Token* name) {
    if (values.find(name->lexeme) != values.end()) {
        return values[name->lexeme];
    }

    if (enclosing != nullptr) return enclosing->get(name);

    throw new RuntimeError(name, "Undefined variable '" + name->lexeme + "'.");
}
Environment* Environment::getEnclosing() {
    return enclosing;
}
void Environment::assign(Token* name, Token* value) {
    auto it = values.find(name->lexeme);
    if (it != values.end()) {
        values[name->lexeme] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }


    throw new RuntimeError(name, "Undefined variable '" + name->lexeme + "'.");
}