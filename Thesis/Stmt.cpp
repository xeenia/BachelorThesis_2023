#include <iostream>
#include "Stmt.h"
#include "Token.h"

BlockStmt::BlockStmt(list<Stmt*> statements) : statements(statements) {}
void BlockStmt::accept(StmtVisitor& visitor) {return visitor.visitBlockStmt(*this); }

ClassStmt::ClassStmt(Token* name, VariableExpr* superclass, list<FunctionStmt*> methods) : name(name), superclass(superclass), methods(methods) {}
void ClassStmt::accept(StmtVisitor& visitor) {return visitor.visitClassStmt(*this); }

ExpressionStmt::ExpressionStmt(Expr* expr) : expr(expr) {}
void ExpressionStmt::accept(StmtVisitor& visitor) {return visitor.visitExpressionStmt(*this); }

FunctionStmt::FunctionStmt(Token* name, vector<Token*> params, list<Stmt*> body) : name(name), params(params), body(body) {}
void FunctionStmt::accept(StmtVisitor& visitor) {return visitor.visitFunctionStmt(*this); }

IfStmt::IfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}
void IfStmt::accept(StmtVisitor& visitor) {return visitor.visitIfStmt(*this); }

PrintStmt::PrintStmt(Expr* expr, bool newLine) : expr(expr), newLine(newLine) {}
void PrintStmt::accept(StmtVisitor& visitor) {return visitor.visitPrintStmt(*this); }

ReadStmt::ReadStmt(vector<Token*> variables, Token* keyword) : variables(variables), keyword(keyword) {}
void ReadStmt::accept(StmtVisitor& visitor) {return visitor.visitReadStmt(*this); }

ReturnStmt::ReturnStmt(Token* keyword, Expr* value) : keyword(keyword), value(value) {}
void ReturnStmt::accept(StmtVisitor& visitor) {return visitor.visitReturnStmt(*this); }

VarStmt::VarStmt(Token* name, Expr* initializer) : name(name), initializer(initializer) {}
void VarStmt::accept(StmtVisitor& visitor) {return visitor.visitVarStmt(*this); }

BreakStmt::BreakStmt(Token* keyword) : keyword(keyword) {}
void BreakStmt::accept(StmtVisitor& visitor) {return visitor.visitBreakStmt(*this); }

ContinueStmt::ContinueStmt(Token* keyword) : keyword(keyword) {}
void ContinueStmt::accept(StmtVisitor& visitor) { return visitor.visitContinueStmt(*this); }

WhileStmt::WhileStmt(Expr* condition, Stmt* body) : condition(condition), body(body) {}
void WhileStmt::accept(StmtVisitor& visitor) {return visitor.visitWhileStmt(*this); }

DoWhileStmt::DoWhileStmt(Expr* condition, Stmt* body) : condition(condition), body(body) {}
void DoWhileStmt::accept(StmtVisitor& visitor) {return visitor.visitDoWhileStmt(*this); }

