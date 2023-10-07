#pragma once

#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include "Expr.h"
#include "tokenType.h"
#include "Stmt.h"
#include "Errors.h"
#include <list>
#include <vector>
class Parser {
public:
	class ParseError {};
	Parser(std::vector<Token*> tokens, Errors* err);
	list<Stmt*> parseStmt();	
private:
	Errors* err;
	std::vector<Token*> tokens;
	int current = 0;

	bool match(std::vector<TokenType> types);
	bool check(TokenType type);
	bool isAtEnd();

	void synchronize();

	Token* advance();
	Token* peek();
	Token* previous();

	Token* consume(TokenType type, std::string message);
	ParseError error(Token* token, std::string message);

	Expr* expression();
	Expr* equality();
	Expr* comparison();
	Expr* term();
	Expr* factor();
	Expr* unary();
	Expr* call();
	Expr* arguments(Expr* callee);
	Expr* primary();
	Expr* assignment();
	Expr* postfixUnary();
	Expr* logicalAnd();
	Expr* logicalOr();

	Stmt* statement();
	Stmt* declaration();
	Stmt* varDeclaration();
	FunctionStmt* function(string kind);
	Stmt* printStatement(bool newLine);
	Stmt* expressionStatement();
	Stmt* ifStatement();
	Stmt* whileStatement();
	Stmt* returnStatement();
	Stmt* doWhileStatement();
	Stmt* breakStatement();
	Stmt* continueStatement();
	Stmt* forStatement();
	Stmt* readStatement();
	Stmt* conditionalStatement();
	
	Stmt* classDeclaration();
	list<Stmt*> block();
	
};


#endif  // PARSER_H
