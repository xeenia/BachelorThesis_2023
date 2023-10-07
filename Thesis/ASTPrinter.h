#pragma once
#ifndef ASTPRINTER_H
#define ASTPRINTER_H
#include "Expr.h"
#include "Stmt.h"
#include <string>
#include <stack>
using namespace std;

class ASTPrinter : ExprVisitor, StmtVisitor{
public:
	void printer(list<Stmt*> stmts);
	string dotContentLabels;
	string dotContentEdges;
private:
	int node(string, string);
	int createNode(string, string);
	void connectEdge(int from, int to);
	stack<int> num;
	int counter;
	
	bool loopFlag;
	vector<string> expressionStr;
	string cleanString(string);
	Token* goToExpr(Expr* expr);
	void goToStmt(Stmt* stmt);
	
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
};
#endif  // ASTPRINTER_H