#pragma once

#ifndef STMT_H
#define STMT_H

#include <string>
#include <iostream>
#include <list> 
#include "Token.h"
#include "Expr.h"
using namespace std; 

class BlockStmt;
class ClassStmt;
class ExpressionStmt;
class FunctionStmt;
class IfStmt;
class PrintStmt;
class ReadStmt;
class ReturnStmt;
class VarStmt;
class BreakStmt;
class ContinueStmt;
class WhileStmt;
class DoWhileStmt;

class StmtVisitor {
public:
    virtual ~StmtVisitor() {}
	virtual void  visitBlockStmt(BlockStmt& stmt) = 0;
	virtual void  visitClassStmt(ClassStmt& stmt) = 0;
	virtual void  visitExpressionStmt(ExpressionStmt& stmt) = 0;
	virtual void  visitFunctionStmt(FunctionStmt& stmt) = 0;
	virtual void  visitIfStmt(IfStmt& stmt) = 0;
	virtual void  visitPrintStmt(PrintStmt& stmt) = 0;
	virtual void  visitReadStmt(ReadStmt& stmt) = 0;
	virtual void  visitReturnStmt(ReturnStmt& stmt) = 0;
	virtual void  visitVarStmt(VarStmt& stmt) = 0;
	virtual void  visitBreakStmt(BreakStmt& stmt) = 0;
	virtual void  visitContinueStmt(ContinueStmt& stmt) = 0;
	virtual void  visitWhileStmt(WhileStmt& stmt) = 0;
	virtual void  visitDoWhileStmt(DoWhileStmt& stmt) = 0;
};

class Stmt{
 public:
	virtual ~Stmt() {}
	virtual void  accept(StmtVisitor& visitor) = 0;
};

class BlockStmt : public Stmt {
public:
	BlockStmt(list<Stmt*> statements);
	list<Stmt*> statements;

	virtual void  accept(StmtVisitor& visitor) override;
};

class ClassStmt : public Stmt {
public:
	ClassStmt(Token* name, VariableExpr* superclass, list<FunctionStmt*> methods);
	Token* name;
	VariableExpr* superclass;
	list<FunctionStmt*> methods;

	virtual void  accept(StmtVisitor& visitor) override;
};

class ExpressionStmt : public Stmt {
public:
	ExpressionStmt(Expr* expr);
	Expr* expr;

	virtual void  accept(StmtVisitor& visitor) override;
};

class FunctionStmt : public Stmt {
public:
	FunctionStmt(Token* name, vector<Token*> params, list<Stmt*> body);
	Token* name;
	vector<Token*> params;
	list<Stmt*> body;

	virtual void  accept(StmtVisitor& visitor) override;
};

class IfStmt : public Stmt {
public:
	IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch);
	Expr* condition;
	Stmt* thenBranch;
	Stmt* elseBranch;

	virtual void  accept(StmtVisitor& visitor) override;
};

class PrintStmt : public Stmt {
public:
	PrintStmt(Expr* expr, bool newLine);
	Expr* expr;
	bool newLine;
	virtual void  accept(StmtVisitor& visitor) override;
};

class ReadStmt : public Stmt {
public:
	ReadStmt(vector<Token*> variables, Token* keyword);
	vector<Token*> variables;
	Token* keyword;
	virtual void  accept(StmtVisitor& visitor) override;
};

class ReturnStmt : public Stmt {
public:
	ReturnStmt(Token* keyword, Expr* value);
	Token* keyword;
	Expr* value;

	virtual void  accept(StmtVisitor& visitor) override;
};

class VarStmt : public Stmt {
public:
	VarStmt(Token* name, Expr* initializer);
	Token* name;
	Expr* initializer;

	virtual void  accept(StmtVisitor& visitor) override;
};

class BreakStmt : public Stmt {
public:
	BreakStmt(Token* keyword);
	Token* keyword;
	virtual void  accept(StmtVisitor& visitor) override;
};

class ContinueStmt : public Stmt {
public:
	ContinueStmt(Token* keyword);
	Token* keyword;
	virtual void  accept(StmtVisitor& visitor) override;
};

class WhileStmt : public Stmt {
public:
	WhileStmt(Expr* condition, Stmt* body);
	Expr* condition;
	Stmt* body;

	virtual void  accept(StmtVisitor& visitor) override;
};

class DoWhileStmt : public Stmt {
public:
	DoWhileStmt(Expr* condition, Stmt* body);
	Expr* condition;
	Stmt* body;

	virtual void  accept(StmtVisitor& visitor) override;
};

#endif  // EXPR_H