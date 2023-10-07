#include "AstPrinter.h"
#define EXPR_COLOR "#f1dca7"
#define STMT_COLOR "#d08c60"
#define LITERAL_COLOR "#9b9b7a"
#define START_COLOR "#997b66"
#define IDENTIFIER_COLOR "#797d62"
#define OPERATOR_COLOR "#d9ae94"
void ASTPrinter::connectEdge(int from, int to) {
	string s = to_string(from) + " -> " + to_string(to) + "; \n";
	dotContentEdges += s;

}
int ASTPrinter::createNode(string str, string color) {
	int current = counter++;

	dotContentLabels += to_string(current) + " [label=\"" + str + "\", fillcolor = \"" + color + "\"];\n";
	return current;

}
int ASTPrinter::node(string type, string color) {
	
		int current = createNode(type, color);
		if (!num.empty()) {
			int parent = num.top();
			connectEdge(parent, current);
		}
		return current;


	return 0;
}


void ASTPrinter::printer(list<Stmt*> stmts) {

	int current = node("Start ", START_COLOR);
	
	for (Stmt* statement : stmts) {
		num.push(current);
		goToStmt(statement);
		num.pop();
	}
	


}


Token* ASTPrinter::goToExpr(Expr* expr) {
	return expr->accept(*this);
}
void ASTPrinter::goToStmt(Stmt* stmt) {
	stmt->accept(*this);
}


string ASTPrinter::cleanString(string str) {
	std::string result = "";

	for (char c : str) {
		if (c != '\"') {
			result += c;
		}
	}
	return result;
}

Token* ASTPrinter::visitAssignExpr(AssignExpr& expr) {
	string t;

	switch (expr.type) {
	case TokenType::PLUSASSIGN:
		t = "+=";
		break;
	case TokenType::MINUSASSIGN:
		t = "-=";
		break;
	case TokenType::STARASSIGN:
		t = "*=";
		break;
	case TokenType::DIVASSIGN:
		t = "/=";
		break;
	default:
		t = "=";
		break;
	}

	int current = node("Assign Expr (" + t + ")", EXPR_COLOR);
	num.push(current);
	current = node("Identifier (" +cleanString(expr.name->lexeme) + ")", IDENTIFIER_COLOR);
	goToExpr(expr.value);
	num.pop();

	return nullptr;
}
Token* ASTPrinter::visitBinaryExpr(BinaryExpr& expr) {
	int current = node("Binary Expr (" + expr.op->lexeme + ")", EXPR_COLOR);
	num.push(current);
	Token* left = goToExpr(expr.left);
	Token* right = goToExpr(expr.right);
	num.pop();
	return nullptr;
}



Token* ASTPrinter::visitCallExpr(CallExpr& expr) {
	int current = node("Call Expr ", EXPR_COLOR);

	num.push(current);
	Token* callee = goToExpr(expr.callee);
	

	vector<Token*> arguments;
	current = node("Arguments ", EXPR_COLOR);
	num.push(current);
	for (Expr* argument : expr.arguments) {
		arguments.push_back(goToExpr(argument));
	}
	num.pop();

	return nullptr;
}
Token* ASTPrinter::visitConditionalExpr(ConditionalExpr& expr) {
	int current = node("Conditional Expr: ", EXPR_COLOR);
	num.push(current);
	Token* condition = goToExpr(expr.condition);
	Token* trueS = goToExpr(expr.trueStmt);
	Token* falseS = goToExpr(expr.falseStmt);
	if (loopFlag) num.pop();
	return nullptr;
}
Token* ASTPrinter::visitGetExpr(GetExpr& expr) {
	int current = node("Get: " + cleanString(expr.name->lexeme), EXPR_COLOR);
	if (loopFlag) num.push(current);
	Token* object = goToExpr(expr.object);
	if (loopFlag) num.pop();
	return nullptr;

}
Token* ASTPrinter::visitGroupingExpr(GroupingExpr& expr) {
	int current = node("Grouping Expr ", EXPR_COLOR);
	num.push(current);
	Token* token = goToExpr(expr.expr);
	num.pop();
	return nullptr;
}
Token* ASTPrinter::visitLiteralExpr(LiteralExpr& expr) {
	node( "Literal (" + cleanString(expr.value->lexeme) + ")", LITERAL_COLOR);
	return nullptr;
}
Token* ASTPrinter::visitLogicalExpr(LogicalExpr& expr) {

	int current = node( "Logical Expr (" +expr.op->lexeme + ")", EXPR_COLOR);
	num.push(current);
	Token* right = goToExpr(expr.right);
	Token* left = goToExpr(expr.left);
	num.pop();
	return nullptr;
}
Token* ASTPrinter::visitSetExpr(SetExpr& expr) {

	int current = node("Set " + cleanString(expr.name->lexeme), EXPR_COLOR);
	num.push(current);
	Token* object = goToExpr(expr.object);
	num.pop();

	num.push(current);
	Token* value = goToExpr(expr.value);
	num.pop();
	return nullptr;
}
Token* ASTPrinter::visitSuperExpr(SuperExpr& expr) {
	return nullptr;
}
Token* ASTPrinter::visitThisExpr(ThisExpr& expr) {
	return nullptr;// lookUpVariable(expr.keyword, &expr);
}
Token* ASTPrinter::visitUnaryExpr(UnaryExpr& expr) {
	int current = node("Unary Expr (" +expr.op->lexeme + ")", EXPR_COLOR);
	num.push(current);
	Token* right = goToExpr(expr.value);
	num.pop();
	
	return nullptr;
}
Token* ASTPrinter::visitVariableExpr(VariableExpr& expr) {
	node("Identifier (" + cleanString(expr.token->lexeme) + ")", IDENTIFIER_COLOR);
	return nullptr;
}


void ASTPrinter::visitBlockStmt(BlockStmt& stmt) {
	int current = node("Block Stmt ", STMT_COLOR);
	num.push(current);
	for (Stmt* statement : stmt.statements) {
		goToStmt(statement);
	}
	num.pop();
}
void ASTPrinter::visitBreakStmt(BreakStmt& stmt) {
	node("Break Stmt ", STMT_COLOR);
}
void ASTPrinter::visitClassStmt(ClassStmt& stmt) {
	Token* superclass = nullptr;
	if (stmt.superclass != nullptr) {
		int current = node("Superclass: " + stmt.superclass->token->lexeme, "#fff1e6");
		 num.push(current);
		superclass = goToExpr(stmt.superclass);
		num.pop();
	}
}
void ASTPrinter::visitContinueStmt(ContinueStmt& stmt) {
	node("Continue Stmt ", STMT_COLOR);
}
void ASTPrinter::visitDoWhileStmt(DoWhileStmt& stmt) {

	int current = node("Do-While Stmt ", STMT_COLOR);
	num.push(current);
	goToStmt(stmt.body);
	goToExpr(stmt.condition);
	num.pop();
}
void ASTPrinter::visitExpressionStmt(ExpressionStmt& stmt) {


	int current= node("Expression Stmt ", STMT_COLOR);
	num.push(current);
	goToExpr(stmt.expr);
	num.pop();
}
void ASTPrinter::visitFunctionStmt(FunctionStmt& stmt) {
	int current = node("Function Stmt (" + cleanString(stmt.name->lexeme) +")", STMT_COLOR);
	
	num.push(current);
	int params = node("Params", STMT_COLOR);
	num.push(params);
	for (Token* param : stmt.params) {
		 node("Param (" + param->lexeme + ")", LITERAL_COLOR);
	}
	num.pop();
	num.pop();
	 
	num.push(current);
	int body = node("Body", STMT_COLOR);
	num.push(body);
	for(Stmt* statement: stmt.body)
		goToStmt(statement);
	num.pop();
}
void ASTPrinter::visitIfStmt(IfStmt& stmt) {

		int current = node("If Stmt", STMT_COLOR);
		num.push(current);
		int condition = node("Condition", STMT_COLOR);
		num.push(condition);
		goToExpr(stmt.condition);
	    num.pop();
		num.pop();
		
		num.push(current);
		int then = node("Then", STMT_COLOR);
		num.push(then);
		goToStmt(stmt.thenBranch);
		num.pop();
		num.pop();
		
		if (stmt.elseBranch != nullptr) {
			num.push(current);
			int else_ = node("Else", STMT_COLOR);
			num.push(else_);
			goToStmt(stmt.elseBranch);
			num.pop();
			num.pop();
		}
		
}
void ASTPrinter::visitPrintStmt(PrintStmt& stmt) {
	int current = node("Print Stmt", STMT_COLOR);
	num.push(current);
	goToExpr(stmt.expr);
	num.pop();
}

void ASTPrinter::visitReadStmt(ReadStmt& stmt) {
	int current = node("Read Stmt", STMT_COLOR);

}
void ASTPrinter::visitReturnStmt(ReturnStmt& stmt) {
	Token* value = NULL;
	int current = node("Return Stmt", STMT_COLOR);
	num.push(current);
	if (stmt.value != NULL) value = goToExpr(stmt.value);
	num.pop();
}
void ASTPrinter::visitVarStmt(VarStmt& stmt) {
	Token* value = nullptr;
	int current = node("Variable Declaration", STMT_COLOR);
	num.push(current);
	current = node("Identifier (" + cleanString(stmt.name->lexeme) + ")", IDENTIFIER_COLOR);
	if (stmt.initializer != nullptr) {
		value = goToExpr(stmt.initializer);
	}
	num.pop();
}
void ASTPrinter::visitWhileStmt(WhileStmt& stmt) {
	
	int current = node("While Stmt", STMT_COLOR);
	num.push(current);
	int condition = node("Condition", STMT_COLOR);
	num.push(condition);
	goToExpr(stmt.condition);
	num.pop();
	int body = node("Body", STMT_COLOR);
	num.push(body);
	goToStmt(stmt.body);
	num.pop();
	num.pop();
	loopFlag = true;

}