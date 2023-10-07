#include <iostream>
#include "Expr.h"
#include "Token.h"

AssignExpr::AssignExpr(Token* name, Expr* value, TokenType type) : name(name), value(value), type(type) {}
Token* AssignExpr::accept(ExprVisitor& visitor) {return visitor.visitAssignExpr(*this); }

BinaryExpr::BinaryExpr(Expr* left, Token* op, Expr* right) : left(left), op(op), right(right) {}
Token* BinaryExpr::accept(ExprVisitor& visitor) {return visitor.visitBinaryExpr(*this); }

CallExpr::CallExpr(Expr* callee, Token* paren, list<Expr*> arguments) : callee(callee), paren(paren), arguments(arguments) {}
Token* CallExpr::accept(ExprVisitor& visitor) {return visitor.visitCallExpr(*this); }

ConditionalExpr::ConditionalExpr(Expr* condition, Expr* trueStmt, Expr* falseStmt) : condition(condition), trueStmt(trueStmt), falseStmt(falseStmt) {}
Token* ConditionalExpr::accept(ExprVisitor& visitor) {return visitor.visitConditionalExpr(*this); }

GetExpr::GetExpr(Expr* object, Token* name) : object(object), name(name) {}
Token* GetExpr::accept(ExprVisitor& visitor) {return visitor.visitGetExpr(*this); }

GroupingExpr::GroupingExpr(Expr* expr) : expr(expr) {}
Token* GroupingExpr::accept(ExprVisitor& visitor) {return visitor.visitGroupingExpr(*this); }

LiteralExpr::LiteralExpr(Token* value) : value(value) {}
Token* LiteralExpr::accept(ExprVisitor& visitor) {return visitor.visitLiteralExpr(*this); }

LogicalExpr::LogicalExpr(Expr* left, Token* op, Expr* right) : left(left), op(op), right(right) {}
Token* LogicalExpr::accept(ExprVisitor& visitor) {return visitor.visitLogicalExpr(*this); }

SetExpr::SetExpr(Expr* object, Token* name, Expr* value) : object(object), name(name), value(value) {}
Token* SetExpr::accept(ExprVisitor& visitor) {return visitor.visitSetExpr(*this); }

SuperExpr::SuperExpr(Token* keyword, Token* method) : keyword(keyword), method(method) {}
Token* SuperExpr::accept(ExprVisitor& visitor) {return visitor.visitSuperExpr(*this); }

ThisExpr::ThisExpr(Token* keyword) : keyword(keyword) {}
Token* ThisExpr::accept(ExprVisitor& visitor) {return visitor.visitThisExpr(*this); }

UnaryExpr::UnaryExpr(Token* op, Expr* value, bool opIsLeft) : op(op), value(value), opIsLeft(opIsLeft) {}
Token* UnaryExpr::accept(ExprVisitor& visitor) {return visitor.visitUnaryExpr(*this); }

VariableExpr::VariableExpr(Token* token) : token(token) {}
Token* VariableExpr::accept(ExprVisitor& visitor) {return visitor.visitVariableExpr(*this); }

