#pragma once

#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include "TokenType.h"
#include "ScriptCallable.h"
#include "ScriptInstance.h"



union LiteralU
{
    int intValue;
    char* stringValue;
    double doubleValue;
    void* nullValue;

    ScriptCallable* callableValue;
    ScriptInstance* instanceValue;
    ScriptClass* classValue;

    explicit LiteralU();
    explicit LiteralU(double _doubleValue);
    explicit LiteralU(void* _nullValue);
    explicit LiteralU(int _intValue);
    explicit LiteralU(string& _stringValue);
    explicit LiteralU(ScriptCallable* _callableValue);
    explicit LiteralU(ScriptInstance* _instanceValue);
    explicit LiteralU(ScriptClass* _classValue);
    ~LiteralU() {}
};
enum ValueType {
    INT, DOUBLE, BOOL, STRING,
    NIL, NONVALUE
};

class Token {
public:
    Token(TokenType type, std::string lexeme, LiteralU* literal, ValueType valueType, int line);
    LiteralU* literal;
    ValueType valueType;

    TokenType type;
    std::string lexeme;
    int line;
};

#endif /* TOKEN_H */

