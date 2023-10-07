#pragma once

#ifndef EXPR_H
#define EXPR_H

#include <string>
#include <iostream>
#include <list> 
#include "Token.h"
using namespace std; 


class AssignExpr;
class BinaryExpr;
class CallExpr;
class ConditionalExpr;
class GetExpr;
class GroupingExpr;
class LiteralExpr;
class LogicalExpr;
class SetExpr;
class SuperExpr;
class ThisExpr;
class UnaryExpr;
class VariableExpr;

class ExprVisitor {
public:
    virtual ~ExprVisitor() {}
	virtual Token*  visitAssignExpr(AssignExpr& expr) = 0;
	virtual Token*  visitBinaryExpr(BinaryExpr& expr) = 0;
	virtual Token*  visitCallExpr(CallExpr& expr) = 0;
	virtual Token*  visitConditionalExpr(ConditionalExpr& expr) = 0;
	virtual Token*  visitGetExpr(GetExpr& expr) = 0;
	virtual Token*  visitGroupingExpr(GroupingExpr& expr) = 0;
	virtual Token*  visitLiteralExpr(LiteralExpr& expr) = 0;
	virtual Token*  visitLogicalExpr(LogicalExpr& expr) = 0;
	virtual Token*  visitSetExpr(SetExpr& expr) = 0;
	virtual Token*  visitSuperExpr(SuperExpr& expr) = 0;
	virtual Token*  visitThisExpr(ThisExpr& expr) = 0;
	virtual Token*  visitUnaryExpr(UnaryExpr& expr) = 0;
	virtual Token*  visitVariableExpr(VariableExpr& expr) = 0;
};

class Expr{
 public:
	virtual ~Expr() {}
	virtual Token*  accept(ExprVisitor& visitor) = 0;
};

class AssignExpr : public Expr {
public:
	AssignExpr(Token* name, Expr* value, TokenType type);
	Token* name;
	Expr* value;
	TokenType type;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class BinaryExpr : public Expr {
public:
	BinaryExpr(Expr* left, Token* op, Expr* right);
	Expr* left;
	Token* op;
	Expr* right;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class CallExpr : public Expr {
public:
	CallExpr(Expr* callee, Token* paren, list<Expr*> arguments);
	Expr* callee;
	Token* paren;
	list<Expr*> arguments;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class ConditionalExpr : public Expr {
public:
	ConditionalExpr(Expr* condition, Expr* trueStmt, Expr* falseStmt);
	Expr* condition;
	Expr* trueStmt;
	Expr* falseStmt;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class GetExpr : public Expr {
public:
	GetExpr(Expr* object, Token* name);
	Expr* object;
	Token* name;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class GroupingExpr : public Expr {
public:
	GroupingExpr(Expr* expr);
	Expr* expr;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class LiteralExpr : public Expr {
public:
	LiteralExpr(Token* value);
	Token* value;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class LogicalExpr : public Expr {
public:
	LogicalExpr(Expr* left, Token* op, Expr* right);
	Expr* left;
	Token* op;
	Expr* right;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class SetExpr : public Expr {
public:
	SetExpr(Expr* object, Token* name, Expr* value);
	Expr* object;
	Token* name;
	Expr* value;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class SuperExpr : public Expr {
public:
	SuperExpr(Token* keyword, Token* method);
	Token* keyword;
	Token* method;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class ThisExpr : public Expr {
public:
	ThisExpr(Token* keyword);
	Token* keyword;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class UnaryExpr : public Expr {
public:
	UnaryExpr(Token* op, Expr* value, bool opIsLeft);
	Token* op;
	Expr* value;
	bool opIsLeft;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

class VariableExpr : public Expr {
public:
	VariableExpr(Token* token);
	Token* token;

	virtual Token*  accept(ExprVisitor& visitor) override;
};

#endif  // EXPR_H