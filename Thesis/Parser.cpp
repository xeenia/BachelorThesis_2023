#include "Parser.h"
#include "Errors.h"
Parser::Parser(std::vector<Token*> tokens, Errors* err) : tokens(tokens), err(err) {}

Stmt* Parser::declaration() {
	try {
		
		if (match({ TokenType::CLASS }))
			return classDeclaration();
		else if (match({ TokenType::FUN }))
			return function("function");
		else if (match({ TokenType::VAR }))
			return varDeclaration();
		return statement();
	}
	catch (ParseError error) {
		synchronize();
		return NULL;
	}
}
Stmt* Parser::classDeclaration() {
	Token* name = consume(TokenType::IDENTIFIER, "Expect class name.");
	VariableExpr* superclass = nullptr;
	if (match({ TokenType::LESS })) {
		consume(TokenType::IDENTIFIER, "Expect superclass name.");
		superclass = new VariableExpr(previous());
	}
	consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

	list<FunctionStmt*> methods;
	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		methods.push_back(function("method"));
	}

	consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");
	
	return new ClassStmt(name, superclass, methods);
}
FunctionStmt* Parser::function(string kind) {
	Token* name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
	consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
	vector<Token*> parameters;
	if (!check(TokenType::RIGHT_PAREN)) {
		do {
			if (parameters.size() >= 255) {
				error(peek(), "Can't have more than 255 parameters.");
			}

			parameters.push_back(
				consume(TokenType::IDENTIFIER, "Expect parameter name."));
		} while (match({ TokenType::COMMA }));
	}

	consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

	consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
	list<Stmt*> body = block();
	return new FunctionStmt(name, parameters, body);
}

Expr* Parser::expression() {
	return assignment();
}

list<Stmt*> Parser::parseStmt() {
	list<Stmt*> stmts;
	while (!isAtEnd()) {

		stmts.push_back(declaration());
	}
	return stmts;
	
}



Stmt* Parser::varDeclaration() {
	Token* name = consume(TokenType::IDENTIFIER, "Expect variable name.");

	Expr* initializer = nullptr;
	Token* assigntype = nullptr;
	if (match({TokenType::ASSIGN})) {
		initializer = expression();
	}
	else if (match({TokenType::PLUSASSIGN ,TokenType::MINUSASSIGN,TokenType::DIVASSIGN, TokenType::MODASSIGN })) {
		consume(TokenType::ASSIGN, "Only allow the use of the \'=\' operator for variable initialization.");
	}
	consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
	Stmt* stmt = new VarStmt(name, initializer);
	
	return stmt;
}



Expr* Parser::assignment() {
	Expr* expr = logicalOr();
	if (match({ TokenType::ASSIGN, TokenType::PLUSASSIGN, TokenType::MINUSASSIGN, TokenType::STARASSIGN, TokenType::DIVASSIGN, TokenType::MODASSIGN })) {
		Token* equalsType = previous();
		Expr* value = assignment();
		if (VariableExpr* varExpr = dynamic_cast<VariableExpr*>(expr)) {
			Token* name = varExpr->token;
			Expr* expr = new AssignExpr(name, value, equalsType->type);
			return expr;
		}
		else if (GetExpr* getExpr = dynamic_cast<GetExpr*>(expr)) {
			SetExpr* setExpr = new SetExpr(getExpr->object, getExpr->name, value);
			return setExpr;
		}
		error(equalsType, "Invalid assignment target.");
	}

	return expr;
}


Expr* Parser::logicalOr() {
	Expr* expr = logicalAnd();
	while (match({ TokenType::LOGICAL_OR })) {
		Token* op = previous();
		Expr* right = logicalAnd();
		expr = new LogicalExpr(expr, op, right);
	}

	return expr;
}

Expr* Parser::logicalAnd() {
	Expr* expr = equality();
	while (match({ TokenType::LOGICAL_AND })) {
		Token* op = previous();
		Expr* right = equality();
		expr = new LogicalExpr(expr, op, right);
	}

	return expr;
}


Expr* Parser::equality() {
	Expr* expr = comparison();
	while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
		Token* op = previous();
		Expr* right = comparison();
		expr = new BinaryExpr(expr, op, right);
	}

	return expr;
}

Expr* Parser::comparison() {
	Expr* expr = term();
	while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL })) {
		Token* op = previous();
		Expr* right = term();
		expr = new BinaryExpr(expr, op, right);
	}

	return expr;
}

Expr* Parser::term() {
	Expr* expr = factor();
	while (match({ TokenType::MINUS ,TokenType::PLUS })) {
		Token* op = previous();
		Expr* right = factor();
		expr = new BinaryExpr(expr, op, right);
	}
	

	return expr;
}

Expr* Parser::factor() {
	Expr* expr = postfixUnary();

	while (match({ TokenType::SLASH ,TokenType::STAR, TokenType::MOD })) {
		Token* op = previous();
		Expr* right = postfixUnary();
		expr = new BinaryExpr(expr, op, right);
	}

	return expr;
}

Expr* Parser::postfixUnary() {
	Expr* expr = unary();
	if (match({ TokenType::PLUSPLUS ,TokenType::MINUSMINUS })) {
		Token* op = previous();
		if (VariableExpr* varExpr = dynamic_cast<VariableExpr*>(expr)) {
			
			Expr* left = expr;
			return new UnaryExpr(op, left, false);
		}else {
			string type = (op->type == TokenType::PLUSPLUS) ? "increment" : "decrement";
			error(peek(), "Identifier required as " + type + " operand.");
		}
	}
	return expr;
}

Expr* Parser::unary() {
	if (match({ TokenType::MINUS ,TokenType::BANG, TokenType::PLUSPLUS ,TokenType::MINUSMINUS, })) {
		Token* op = previous();
		Expr* right = unary();
		if(op->type == TokenType::PLUSPLUS || op->type == TokenType::MINUSMINUS)
			if(VariableExpr* varExpr = dynamic_cast<VariableExpr*>(right))
				return new UnaryExpr(op, right, true);
			else {
				string type = (op->type == TokenType::PLUSPLUS) ? "increment" : "decrement";
				error(peek(), "Identifier required as " + type + " operand.");
			}
		return new UnaryExpr(op, right, true);
	}


	return call(); 
}

Expr* Parser::call() {
	Expr* expr = primary();
	while (true) {
		if (match({ TokenType::LEFT_PAREN})) {
			expr = arguments(expr);
			continue;
		}
		if (match({ TokenType::DOT })) {
			Token* name = consume(TokenType::IDENTIFIER,
				"Expect property name after '.'.");
			expr = new GetExpr(expr, name);
			continue;
		}
		break;

	}

	return expr;
}

Expr* Parser::arguments(Expr* callee) {
	list<Expr*> arguments;
	if (!check(TokenType::RIGHT_PAREN)) {
		do {
			if (arguments.size() >= 255) {
				error(peek(), "Can't have more than 255 arguments.");
			}
			arguments.push_back(expression());
		} while (match({TokenType::COMMA}));
	}

	Token* paren = consume(TokenType::RIGHT_PAREN,
		"Expect ')' after arguments.");

	return new CallExpr(callee, paren, arguments);
}


Expr* Parser::primary() {

	if (match({ TokenType::FALSE })) return new LiteralExpr(new Token(TokenType::FALSE, "false", new LiteralU(0), ValueType::BOOL, 0));

	else if (match({ TokenType::TRUE })) return new LiteralExpr(new Token(TokenType::TRUE, "true", new LiteralU(1), ValueType::BOOL,0));

	else if (match({ TokenType::NIL })) return new LiteralExpr(new Token(TokenType::NIL, "NIL", nullptr, ValueType::NIL, 0));

	else if (match({ TokenType::NUMBER })) return new LiteralExpr(previous());

	else if (match({ TokenType::STRING })) return new LiteralExpr(previous());

	else if (match({ TokenType::SUPER })) {
		Token* keyword = previous();
		consume(TokenType::DOT, "Expect '.' after 'super'.");
		Token* method = consume(TokenType::IDENTIFIER,
			"Expect superclass method name.");
		return new SuperExpr(keyword, method);
	}

	else if (match({ TokenType::THIS })) return new ThisExpr(previous());

	else if (match({ TokenType::IDENTIFIER })) {
		Expr* expr = new VariableExpr(previous());
		return expr;
	}

	else if (match({ TokenType::LEFT_PAREN })) {
		Expr* expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
		if (peek()->type == TokenType::QUESTION_MARK) {
			consume(TokenType::QUESTION_MARK, "Expect '?' after the symbol ')'.");
			Expr* trueStmt = expression();
			consume(TokenType::COLON, "Expect ':' after expression.");
			Expr* falseStmt = expression();
			return new ConditionalExpr(expr, trueStmt, falseStmt);
		}
		return new GroupingExpr(expr);
	}

	throw error(peek(), "Expect expression.");

}
Stmt* Parser::breakStatement(){
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return new BreakStmt(previous());
}

Stmt* Parser::continueStatement() {
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return new ContinueStmt(previous());
}



Stmt* Parser::statement() {
	
	bool newLine;
	if (peek()->type == TokenType::PRINT) newLine = false;
	else newLine = true;

	if (match({ TokenType::BREAK })) {
		 return breakStatement();
	}

	else if (match({ TokenType::CONTINUE })) {
		return continueStatement();
	}
	else if (match({ TokenType::PRINT, TokenType::PRINTNL })) return printStatement(newLine);

	else if (match({ TokenType::READ })) return readStatement();
	else if (match({ TokenType::FOR })) return forStatement();

	else if (match({ TokenType::DO })) return doWhileStatement();
	else if (match({ TokenType::IF })) return ifStatement();
	else if (match({ TokenType::RETURN })) return returnStatement();

	else if (match({ TokenType::LEFT_BRACE })) return new BlockStmt(block());
	else if (match({ TokenType::WHILE })) return whileStatement();
	return expressionStatement();
}
Stmt* Parser::returnStatement() {
	Token* keyword = previous();
	Expr* value = NULL;
	if (!check(TokenType::SEMICOLON)) {
		value = expression();
	}

	consume(TokenType::SEMICOLON, "Expect ';' after return value.");
	return new ReturnStmt(keyword, value);
}


Stmt*  Parser::forStatement() {
	consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");	
	Stmt* initializer;
	if (match({ TokenType::SEMICOLON })) {
		initializer = nullptr;
	}
	else if (match({ TokenType::VAR })) {
		initializer = varDeclaration();
	}
	else {
		initializer = expressionStatement();
	}

	Expr* condition = nullptr;
	if (!check(TokenType::SEMICOLON)) {
		condition = expression();
	}
	consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

	Expr* increment = NULL;
	if (!check(TokenType::RIGHT_PAREN)) {
		increment = expression();
	}
	consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");
	Stmt* body = statement();

	if (increment != nullptr) {
		list<Stmt*> statements = { body, new ExpressionStmt(increment) };
		body = new BlockStmt(statements);
	}

	if (condition == nullptr) condition = new LiteralExpr(new Token(TokenType::TRUE,"true",new LiteralU(1), ValueType::BOOL, 0));
	body = new WhileStmt(condition, body);

	if (initializer != nullptr) {
		list<Stmt*> statements = { initializer, body };
		body = new BlockStmt(statements);
	}
	return body;

}


list<Stmt*> Parser::block() {
	list<Stmt*> statements;

	while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
		statements.push_back(declaration());
	}

	consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
	return statements;
}
Stmt* Parser::printStatement(bool newLine) {
	consume(TokenType::COLON, "Expect ':' after print statement.");
	Expr* value = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	Stmt* stmt = new PrintStmt(value, newLine);
	return stmt;
}


Stmt* Parser::readStatement() {
	consume(TokenType::COLON, "Expect ':' after read statement.");
	vector<Token*> variables;
	if (!check(TokenType::SEMICOLON)) {
		do {
			if (variables.size() >= 255) {
				error(peek(), "Can't have more than 255 variables.");
			}

			variables.push_back(
			consume(TokenType::IDENTIFIER, "Expect variable name."));
		} while (match({ TokenType::COMMA }));
	}
	consume(TokenType::SEMICOLON, "Expect ';' at the end of read statement.");
	return new ReadStmt(variables, new Token(TokenType::READ, "read", nullptr, ValueType::NONVALUE, 0));
}
Stmt* Parser::ifStatement() {
	consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
	Expr* condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

	Stmt* thenBranch = statement();
	Stmt* elseBranch = nullptr;
	if (match({ TokenType::ELSE })) {
		elseBranch = statement();
	}
	Stmt* stmt = new IfStmt(condition, thenBranch, elseBranch);
	return stmt;
}


Stmt* Parser::conditionalStatement() {
	consume(TokenType::LEFT_PAREN, "Expect '('.");
	Expr* condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expect ')'.");

	Stmt* thenBranch = statement();
	Stmt* elseBranch = nullptr;

	if (match({ TokenType::ELSE })) {
		elseBranch = statement();
	}
	Stmt* stmt = new IfStmt(condition, thenBranch, elseBranch);
	return stmt;
}

Stmt* Parser::expressionStatement() {
	Expr* expr = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	Stmt* stmt =  new ExpressionStmt(expr);
	return stmt;
}

bool Parser::match(std::vector<TokenType> types) {
	for (TokenType type : types) {
		if (check(type)) {
			advance();
			return true;
		}
	}
	return false;

}

bool Parser::check(TokenType type) {
	if (isAtEnd()) return false;
	return peek()->type == type;
}

Token* Parser::advance() {
	if (!isAtEnd()) current++;
	return previous();
}

bool Parser::isAtEnd() {
	return peek()->type == TokenType::END_OF_FILE;
}

Token* Parser::peek() {
	return tokens[current];
}

Token* Parser::previous() {
	return tokens[current - 1];
}

Token* Parser::consume(TokenType type, std::string message) {
	if (check(type)) return advance();

	throw error(peek(), message);
}

Parser::ParseError Parser::error(Token* token, std::string message) {
	err->error(token, message);
	ParseError* perror = new ParseError();
	return *perror;
}

void Parser::synchronize() {
	advance();

	while (!isAtEnd()) {
		if (previous()->type == TokenType::SEMICOLON) return;

		switch (peek()->type) {
		case TokenType::CLASS:
		case TokenType::FUN:
		case TokenType::VAR:
		case TokenType::FOR:
		case TokenType::IF:
		case TokenType::WHILE:
		case TokenType::PRINT:
		case TokenType::PRINTNL:
		case TokenType::RETURN:
		case TokenType::BREAK:
		case TokenType::CONTINUE:
		case TokenType::DO:
		case TokenType::READ:
			return;
		}

		advance();
	}
}


Stmt* Parser::whileStatement() {
	consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
	Expr* condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");

	Stmt* body = statement();

	return new WhileStmt(condition, body);
}

Stmt* Parser::doWhileStatement() {
	Stmt* body = statement();
	consume(TokenType::WHILE, "Expect 'while' after condition.");
	consume(TokenType::LEFT_PAREN, "Expect 'WHILE' after condition.");
	Expr* condition = expression();
	consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
	consume(TokenType::SEMICOLON, "Expect ';' after while.");

	return new DoWhileStmt(condition, body);
}