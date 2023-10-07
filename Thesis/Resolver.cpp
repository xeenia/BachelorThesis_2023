#include "Resolver.h"
#include "Token.h"


Resolver::Resolver(Interpreter* interpreter, Errors* err) : interpreter(interpreter), err(err), loopFlag(false) {}

void Resolver::resolve(list<Stmt*> statements) {

    for (Stmt* statement : statements) {
        resolve(statement);
    }
}

void Resolver::resolve(Stmt* stmt) {
    stmt->accept(*this);
}

void Resolver::beginScope() {
    scopes.push_back(map<string, bool>());
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token* name) {
    if (scopes.empty() ==  true) return;
    map<string, bool>* scope = &scopes.back();

    if (scope->find(name->lexeme) != scope->end()) {
          err->error(name, "Already a variable with this name in this scope.");
    }
    (*scope)[name->lexeme] = false;
}

void Resolver::define(Token* name) {
    if (scopes.empty()==true) return;
    scopes.back()[name->lexeme] = true;
}

void Resolver::resolveLocal(Expr* expr, Token* name) {
    int s = scopes.size();
    for (int i = scopes.size() - 1; i >= 0; i--) {
        if (scopes[i].find(name->lexeme) != scopes[i].end()) {
            interpreter->resolve(expr, scopes.size() - 1 - i);
            return;
        }
    }
}

void Resolver::visitBlockStmt(BlockStmt& stmt) {
    beginScope();
    resolve(stmt.statements);
    endScope();
}
void Resolver::visitClassStmt(ClassStmt& stmt) {
    ClassType enclosingClass = currentClass;
    currentClass = ClassType::CLASS;
    declare(stmt.name);
    define(stmt.name);
    if (stmt.superclass != nullptr &&
        stmt.name->lexeme == stmt.superclass->token->lexeme) {
        err->error(stmt.superclass->token, "A class can't inherit from itself.");
    }
    if (stmt.superclass != nullptr) {
        currentClass = ClassType::SUBCLASS;
        resolve(stmt.superclass);
    }

    if (stmt.superclass != nullptr) {
        beginScope();
        scopes.back()["super"] = true; 
    }
    beginScope();
    scopes.back()["this"] = true;
    for (FunctionStmt* method : stmt.methods) {
        FunctionType declaration = FunctionType::METHOD;
        if (method->name->lexeme == "init") {
            declaration = FunctionType::INITIALIZER;
        }
        resolveFunction(*method, declaration);
    }
    endScope();
    if (stmt.superclass != nullptr) endScope();
    currentClass = enclosingClass;

    return;
}
Token* Resolver::visitSuperExpr(SuperExpr& expr) {
    if (currentClass == ClassType::NONECLASS) {
        err->error(expr.keyword,"Can't use 'super' outside of a class.");
    }
    else if (currentClass != ClassType::SUBCLASS) {
        err->error(expr.keyword, "Can't use 'super' in a class with no superclass.");
    }
    resolveLocal(&expr, expr.keyword);
    return 0;
}
void Resolver::visitVarStmt(VarStmt& stmt) {
    declare(stmt.name);

    if (stmt.initializer != NULL) {
        resolve(stmt.initializer);
    }
    define(stmt.name);
}

void Resolver::resolve(Expr* expr) {
    expr->accept(*this);
}
Token* Resolver::visitThisExpr(ThisExpr& expr) {
    if (currentClass == ClassType::NONECLASS) {
        err->error(expr.keyword, "Can't use 'this' outside of a class.");
        return nullptr;
    }
    resolveLocal(&expr, expr.keyword);
    return nullptr;
}


Token* Resolver::visitVariableExpr(VariableExpr& expr) {
  
    if (!scopes.empty() &&
        scopes.back().find(expr.token->lexeme) != scopes.back().end() &&
        scopes.back().at(expr.token->lexeme) == false) {
        err->error(expr.token, "Can't read local variable in its own initializer.");
    }

    resolveLocal(&expr, expr.token);
    return 0;
}
Token* Resolver::visitAssignExpr(AssignExpr& expr) {
    resolve(expr.value);
    resolveLocal(&expr, expr.name);
    return 0;
}
void Resolver::visitFunctionStmt(FunctionStmt& stmt) {
    declare(stmt.name);
    define(stmt.name);

    resolveFunction(stmt, FunctionType::FUNCTION);
}

void Resolver::resolveFunction(FunctionStmt& function, FunctionType type) {
    FunctionType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    for (Token* param : function.params) {
        declare(param);
        define(param);
    }
    resolve(function.body);
    endScope();
    currentFunction = enclosingFunction;
}

void Resolver::visitExpressionStmt(ExpressionStmt& stmt) {
    resolve(stmt.expr);
}

void Resolver::visitIfStmt(IfStmt& stmt) {
    resolve(stmt.condition);
    resolve(stmt.thenBranch);
    if (stmt.elseBranch != nullptr) resolve(stmt.elseBranch);
}
Token* Resolver::visitConditionalExpr(ConditionalExpr& expr) {
    resolve(expr.condition);
    resolve(expr.trueStmt);
    resolve(expr.falseStmt);
    return nullptr;
}
void Resolver::visitPrintStmt(PrintStmt& stmt) {
    resolve(stmt.expr);
}

void Resolver::visitReadStmt(ReadStmt& stmt) {
    if (stmt.variables.size() == 0) err->error(stmt.keyword, "Can't use read statement without any variable to store.");
}

void Resolver::visitReturnStmt(ReturnStmt& stmt) {
    if (currentFunction == FunctionType::NONE) {
        err->error(stmt.keyword, "Can't return from top-level code.");
    }
    if (stmt.value != nullptr) {
        if (currentFunction == FunctionType::INITIALIZER) {
            err->error(stmt.keyword, "Can't return a value from an initializer.");
        }
        resolve(stmt.value);
    }

}

void Resolver::visitBreakStmt(BreakStmt& stmt) {
    if(!loopFlag) err->error(stmt.keyword, "Can't add break statement outside from loop statement.");
}

void Resolver::visitContinueStmt(ContinueStmt& stmt) {
    if (!loopFlag) err->error(stmt.keyword, "Can't add continue statement outside from loop statement.");
}

void Resolver::visitWhileStmt(WhileStmt& stmt) {
    loopFlag = true;
    resolve(stmt.condition);
    resolve(stmt.body);
    loopFlag = false;
}

void Resolver::visitDoWhileStmt(DoWhileStmt& stmt) {
    loopFlag = true;
    resolve(stmt.body);
    resolve(stmt.condition);
    loopFlag = false;
}

Token* Resolver::visitBinaryExpr(BinaryExpr& expr) {
    resolve(expr.left);
    resolve(expr.right);
    return NULL;
}

Token* Resolver::visitCallExpr(CallExpr& expr) {
    resolve(expr.callee);

    for (Expr* argument : expr.arguments) {
        resolve(argument);
    }

    return NULL;
}
Token* Resolver::visitGetExpr(GetExpr& expr) {
    resolve(expr.object);
    return NULL;
}

Token* Resolver::visitSetExpr(SetExpr& expr) {
    resolve(expr.value);
    resolve(expr.object);
    return 0;
}

Token* Resolver::visitGroupingExpr(GroupingExpr& expr) {
    resolve(expr.expr);
    return NULL;
}

Token* Resolver::visitLiteralExpr(LiteralExpr& expr) {
    return NULL;
}

Token* Resolver::visitLogicalExpr(LogicalExpr& expr) {
    resolve(expr.left);
    resolve(expr.right);
    return NULL;
}

Token* Resolver::visitUnaryExpr(UnaryExpr& expr) {
    resolve(expr.value);
    return NULL;
}