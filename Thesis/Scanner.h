#pragma once

#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include "TokenType.h"
#include "Token.h"
#include "Errors.h"
#include <map>
class Scanner {

public:
    Scanner(std::string source, Errors* err);
    std::vector<Token*> scanTokens();
private:
    const std::string source;
    std::vector<Token*> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    Errors* err;

    bool isInt(const std::string& str);
    bool isAtEnd();
    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, LiteralU* literal, ValueType valueType);
    bool match(char expected);
    char peek();
    void string();
    bool isDigit(char c);
    void number();
    char peekNext();
    void identifier();
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    void comment();

    static std::map<std::string, TokenType> keywords;

};

#endif /* SCANNER_H */
