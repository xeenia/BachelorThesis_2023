#pragma once
#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "Expr.h"
#include "Stmt.h"
#include "ScriptCallable.h"
#include <unordered_map>
#include "Errors.h"
#include <iostream>
using namespace std;
#include <stack>
#include <vector>
#include <string>

class Environment;

class Interpreter : public ExprVisitor,StmtVisitor {
public:
	Environment* const globals;
	
	Token* visitGroupingExpr(GroupingExpr& expr) override;
	Token* visitBinaryExpr(BinaryExpr& expr) override;
	Token* visitUnaryExpr(UnaryExpr& expr) override;
	Token* visitLiteralExpr(LiteralExpr& expr) override;
	Token* visitVariableExpr(VariableExpr& expr) override;
	Token* visitCallExpr(CallExpr& expr) override;
	Token* visitGetExpr(GetExpr& expr) override;
	Token* visitAssignExpr(AssignExpr& expr) override;
	Token* visitLogicalExpr(LogicalExpr& expr) override;
	Token* visitConditionalExpr(ConditionalExpr& expr) override;
	Token* visitSetExpr(SetExpr& expr) override;
	Token* visitThisExpr(ThisExpr& expr) override;
	Token* visitSuperExpr(SuperExpr& expr) override;
 
    void visitExpressionStmt(ExpressionStmt& stmt) override;
	void visitBreakStmt(BreakStmt& stmt) override;
	void visitContinueStmt(ContinueStmt& stmt) override;
	void visitFunctionStmt(FunctionStmt& stmt) override;
	void visitPrintStmt(PrintStmt& stmt) override;
	void visitReadStmt(ReadStmt& stmt) override;
	void visitIfStmt(IfStmt& stmt) override;
	void visitClassStmt(ClassStmt& stmt) override;
	void visitVarStmt(VarStmt& stmt) override;
	void visitReturnStmt(ReturnStmt& stmt) override;
	void visitBlockStmt(BlockStmt& stmt) override;
	void visitWhileStmt(WhileStmt& stmt) override;
	void visitDoWhileStmt(DoWhileStmt& stmt) override;
	


	void interpret(Expr* expr);
	void interpret(list<Stmt*> stmts);
	void resolve(Expr* expr, int depth);
	void executeBlock(list<Stmt*> statements, Environment* newEnv);
	Interpreter(Errors* err);
	
private:
	Environment* environment;
	std::map<Expr*, int> locals;

	Errors* err;
	string cleanString(string);
	
	
	Token* evaluate(Expr* expr);
	void execute(Stmt* stmt);

	bool isTruthy(Token* expr);
	bool isEqual(Token* a, Token* b);

	void checkNumberOperand(Token op, Token* operand);
	void checkNumberOperands(Token op, Token* left, Token* right);

	string stringify(Token* value);
	
	bool breakActivated;
	bool continueActivated;

	bool isInteger(string& str);
	LiteralU* convertToLiteral(Token* token);

	Token* lookUpVariable(Token* name, Expr* expr);
	

	
	
};
#endif  // INTERPRETER_H
