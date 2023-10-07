#pragma once
#ifndef RESOLVER_H
#define RESOLVER_H
#include "Expr.h"
#include "Interpreter.h"
#include "Stmt.h"
#include <stack>
#include <map>
#include "Errors.h"
class Resolver : ExprVisitor, StmtVisitor {
private:
    Interpreter* interpreter;
    vector<map<string, bool>> scopes;
    enum FunctionType {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD
    };
    enum ClassType {
        NONECLASS,
        CLASS,
        SUBCLASS
    };

    ClassType currentClass = ClassType::NONECLASS;
    FunctionType currentFunction = FunctionType::NONE;
    bool loopFlag;
    void resolve(Stmt* stmt);
    void resolve(Expr* expr);
    void beginScope();
    void endScope();
    void declare(Token* name);
    void define(Token* name);
    void resolveFunction(FunctionStmt& function, FunctionType type);
    void resolveLocal(Expr* expr, Token* name);
    Errors* err;
public:
    Resolver(Interpreter* interpreter, Errors* err);
    void resolve(list<Stmt*> statements);

    void visitBlockStmt(BlockStmt& stmt) override;
    void visitClassStmt(ClassStmt& stmt) override;
    void visitVarStmt(VarStmt& stmt) override;
    void visitFunctionStmt(FunctionStmt& stmt) override;
    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitPrintStmt(PrintStmt& stmt) override;
    void visitReadStmt(ReadStmt& stmt) override;
    void visitReturnStmt(ReturnStmt& stmt) override;
    void visitBreakStmt(BreakStmt& stmt) override;
    void visitContinueStmt(ContinueStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;
    void visitDoWhileStmt(DoWhileStmt& stmt) override;
    Token* visitBinaryExpr(BinaryExpr& expr) override;
    Token* visitCallExpr(CallExpr& expr) override;
    Token* visitGroupingExpr(GroupingExpr& expr) override;
    Token* visitLiteralExpr(LiteralExpr& expr) override;
    Token* visitLogicalExpr(LogicalExpr& expr) override;
    Token* visitUnaryExpr(UnaryExpr& expr) override;
    Token* visitGetExpr(GetExpr& expr) override;
    Token* visitConditionalExpr(ConditionalExpr& expr) override;
    Token* visitSetExpr(SetExpr& expr) override;
    Token* visitThisExpr(ThisExpr& expr) override;
    Token* visitSuperExpr(SuperExpr& expr) override;
    Token* visitVariableExpr(VariableExpr& expr) override;
    Token* visitAssignExpr(AssignExpr& expr) override;


};

#endif