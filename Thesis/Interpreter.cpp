#include "Interpreter.h"
#include "Environment.h"
#include "ScriptFunction.h"
#include "RuntimeError.h"
#include <exception>
#include "Errors.h"

//Constructor
Interpreter::Interpreter(Errors* err) : globals(new Environment()), breakActivated(false), continueActivated(false){
	this->err = err;
	environment = globals;
}

//Checking number Operand
void Interpreter::checkNumberOperand(Token op, Token* operand) {
	switch (operand->valueType) {
	case ValueType::INT:
		break;
	case ValueType::DOUBLE:
		break;
	case ValueType::BOOL:
		break;
	default:
		throw  new RuntimeError(&op, "Operand must be a number.");
	}
	
}
void Interpreter::checkNumberOperands(Token op, Token* left, Token* right) {
	switch (left->valueType) {
	case ValueType::INT:
		break;
	case ValueType::DOUBLE:
		break;
	case ValueType::BOOL:
		break;
	default:
		throw  new RuntimeError(&op, "Operands must be numbers.");
	}

	switch (right->valueType) {
	case ValueType::INT:
		break;
	case ValueType::DOUBLE:
		break;
	case ValueType::BOOL:
		break;
	default:
		throw  new RuntimeError(&op, "Operands must be numbers.");
	}
	
	
}

string Interpreter::cleanString(string str) {
	std::string result = "";

	for (char c : str) {
		if (c != '\"') {
			result += c;
		}
	}
	return result;
}

//Visitor pattern function
Token* Interpreter::evaluate(Expr* expr) {
	return expr->accept(*this);
}
void Interpreter::execute(Stmt* stmt) {
	stmt->accept(*this);
}
void Interpreter::executeBlock(list<Stmt*> statements, Environment* newEnv) {
	Environment* previous = this->environment;
	try {
		this->environment = newEnv;

		for (Stmt* statement : statements) {
			if (breakActivated || continueActivated) {
				continueActivated = false;
				break;
			}
			execute(statement);
		}
	}
	catch (...) {

		this->environment = previous;
		throw;

	}
	this->environment = previous;
}

//Executing the statements
void Interpreter::interpret(list<Stmt*> stmts) {

	try {
		for (Stmt* statement : stmts) {

			execute(statement);
		}
	}
	catch (RuntimeError* error) {
		err->runtimeError(error);
	}


}
void Interpreter::interpret(Expr* expr) {
	try {

		Token* value = evaluate(expr);
	}
	catch (RuntimeError* error) {
		err->runtimeError(error);
	}
}

bool Interpreter::isEqual(Token* a, Token* b) {
	if (a->valueType == ValueType::STRING && b->valueType == ValueType::STRING) {
		return a->literal->stringValue == b->literal->stringValue;
	}
	else if (a->valueType == ValueType::NIL && b->valueType == ValueType::NIL) {
		return true;
	}


	double left= numeric_limits<double>::quiet_NaN(), right= numeric_limits<double>::quiet_NaN();
	if (a->valueType == ValueType::INT || a->valueType == ValueType::BOOL) 
		left = a->literal->intValue;
	else if(a->valueType == ValueType::DOUBLE) 
		left = a->literal->doubleValue;

	if (b->valueType == ValueType::INT || b->valueType == ValueType::BOOL) 
		right = b->literal->intValue;
	else if (b->valueType == ValueType::DOUBLE)  
		right = b->literal->doubleValue;

	if (!isnan(left) && !isnan(right)) {
		return left == right;
		
	}

	return false;

	
}
bool Interpreter::isTruthy(Token* expr) {
	switch (expr->valueType) {
	case ValueType::STRING:
		return true;
		break;
	case ValueType::INT:
	case ValueType::BOOL:
		return expr->literal->intValue;
		break;
	case ValueType::DOUBLE:
		return expr->literal->doubleValue;
		break;
	case ValueType::NIL:
		return false;
		break;
	default:
		return true;
	}
}

void Interpreter::resolve(Expr* expr, int depth) {
	locals[expr] = depth;
}

string Interpreter::stringify(Token* value) {
	string s;
	if (value->valueType == ValueType::NONVALUE) {
		switch (value->type) {
		case TokenType::CLASS: 
			return value->literal->classValue->toString();
		case TokenType::CALLABLE:
			return value->literal->callableValue->toString();
		case TokenType::INSTANCE:
			return value->literal->instanceValue->toString();
		case TokenType::NEW_LINE:
			return "\n";
		}
	}
	switch (value->valueType) {
	case ValueType::INT:
		return to_string(value->literal->intValue);
	case ValueType::DOUBLE:
		return to_string(value->literal->doubleValue);
	case ValueType::BOOL:
		return (value->literal->intValue) ? "true" : "false";
	case ValueType::NIL:
		return "nil";
	}
	return value->literal->stringValue;
}

LiteralU* Interpreter::convertToLiteral(Token* token) {
	LiteralU* l = nullptr;
	switch (token->valueType)
	{
	case ValueType::INT:
		l = new LiteralU(token->literal->intValue);
		break;
	case ValueType::BOOL:
		l = new LiteralU(token->literal->intValue);
		break;
	case ValueType::DOUBLE:
		l = new LiteralU(token->literal->doubleValue);
		break;
	case ValueType::STRING:
		l = new LiteralU(token->literal->stringValue);
		break;
	default:
		if (token->type == TokenType::CLASS) {
			l = new LiteralU(token->literal->classValue);
		}
		break;
	}

	return l;
}

bool isInt(ValueType type) {
	return type == ValueType::INT;
}


//Expr visitors
Token* Interpreter::visitAssignExpr(AssignExpr& expr) {
	Token* currentValue = lookUpVariable(expr.name, &expr);

	Token* value = evaluate(expr.value);

	if (currentValue != nullptr && expr.type == TokenType::PLUSASSIGN && currentValue->type == TokenType::STRING && value->type == TokenType::STRING) {
		string fin = currentValue->literal->stringValue;
		fin.append(value->literal->stringValue);
		value = new Token(expr.type, "string", new LiteralU(fin), ValueType::STRING, 0);
	}
	else if(expr.type != TokenType::ASSIGN){
		checkNumberOperands(*expr.name, currentValue, value);
		double currentVal = (currentValue->valueType == ValueType::DOUBLE) ? currentValue->literal->doubleValue : static_cast<double>(currentValue->literal->intValue);
		double nextVal = (value->valueType == ValueType::DOUBLE) ? value->literal->doubleValue : static_cast<double>(value->literal->intValue);
		switch (expr.type) {
		case TokenType::PLUSASSIGN:
			currentVal += nextVal;
			break;
		case TokenType::MINUSASSIGN:
			currentVal -= nextVal;
			break;
		case TokenType::STARASSIGN:
			currentVal *= nextVal;
			break;
		case TokenType::DIVASSIGN:
			if(nextVal == 0.0) throw  new RuntimeError(expr.name, "Unexpected divide by zero.");
			currentVal /= nextVal;
			break;
		case TokenType::MODASSIGN:
			currentVal = fmod(currentVal, nextVal);
			break;
		}

		bool isInt_b = std::trunc(currentVal) == currentVal;
		if (isInt_b) value->literal->intValue = static_cast<int>(currentVal);
		else  value->literal->doubleValue = currentVal;
	}

	int distance;
	if (locals.find(&expr) != locals.end()) {
		distance = locals[&expr];
		environment->assignAt(distance, expr.name, value);
	}
	else {
		globals->assign(expr.name, value);
	}


	return value;
}
Token* Interpreter::visitBinaryExpr(BinaryExpr& expr) {

	Token* left = evaluate(expr.left);
	Token* right = evaluate(expr.right);

	double leftV;
	double rightV;

	bool isInt_b;
	double result = 0.0;

	LiteralU* finalLiteral;

	switch (expr.op->type) {
	case TokenType::BANG_EQUAL:
		return new Token(expr.op->type, expr.op->lexeme, new LiteralU(!isEqual(left, right)), ValueType::BOOL, 0);
	case TokenType::EQUAL_EQUAL:
		return new Token(expr.op->type, expr.op->lexeme, new LiteralU(isEqual(left, right)), ValueType::BOOL, 0);
	case TokenType::GREATER:
	case TokenType::GREATER_EQUAL:
	case TokenType::LESS:
	case TokenType::LESS_EQUAL:
		checkNumberOperands(*expr.op, left, right);
		
		leftV = (isInt(left->valueType)) ? static_cast<double>(left->literal->intValue) : left->literal->doubleValue;
		rightV = (isInt(right->valueType)) ? static_cast<double>(right->literal->intValue) : right->literal->doubleValue;
		if (expr.op->type == TokenType::GREATER)
			result = leftV > rightV;
		else if (expr.op->type == TokenType::GREATER_EQUAL)
			result = leftV >= rightV;
		else if (expr.op->type == TokenType::LESS)
			result = leftV < rightV;
		else if (expr.op->type == TokenType::LESS_EQUAL)
			result = leftV <= rightV;
		
		return new Token(expr.op->type, expr.op->lexeme, new LiteralU(static_cast<int>(result)), ValueType::BOOL, 0);
	case TokenType::MINUS:
	case TokenType::SLASH:
	case TokenType::STAR:
	case TokenType::MOD:
		checkNumberOperands(*expr.op, left, right);
		leftV = (isInt(left->valueType)) ? static_cast<double>(left->literal->intValue) : left->literal->doubleValue;
		rightV = (isInt(right->valueType)) ? static_cast<double>(right->literal->intValue) : right->literal->doubleValue;

		if (expr.op->type == TokenType::MINUS)
			result = leftV - rightV;
		else if (expr.op->type == TokenType::SLASH) {
			if (rightV == 0) throw  new RuntimeError(expr.op, "Unexpected divide by zero.");
			result = leftV / rightV;
		}
		else if (expr.op->type == TokenType::STAR)
			result = leftV * rightV;
		else if (expr.op->type == TokenType::MOD)
			result = fmod(leftV, rightV);
		
		isInt_b = std::trunc(result) == result;
		if (isInt_b) finalLiteral = new LiteralU(static_cast<int>(result));
		else  finalLiteral = new LiteralU(result);

		return new Token(expr.op->type, expr.op->lexeme,finalLiteral, (isInt_b) ? ValueType::INT : ValueType::DOUBLE, 0);
	case TokenType::PLUS:
		string rightStr, leftStr, finalStr; 
		if (left != nullptr && right != nullptr) {
			if (left->valueType == ValueType::STRING && right->valueType == ValueType::STRING) {
				rightStr = right->literal->stringValue;
				leftStr = left->literal->stringValue;
				finalStr = leftStr + rightStr;
			}
			else if (left->valueType == ValueType::STRING) {
				leftStr = left->literal->stringValue;
				rightStr = stringify(right);
				finalStr = leftStr + rightStr;
			}
			else if (right->valueType == ValueType::STRING) {
				rightStr = right->literal->stringValue;
				leftStr = stringify(left);
				finalStr = leftStr + rightStr;
			}
			else {
				checkNumberOperands(*expr.op, left, right);
				leftV = (isInt(left->valueType)) ? left->literal->intValue : left->literal->doubleValue;
				rightV = (isInt(right->valueType)) ? right->literal->intValue : right->literal->doubleValue;

				result = leftV + rightV;
				isInt_b = std::trunc(result) == result;
				
				if (isInt_b) finalLiteral = new LiteralU(static_cast<int>(result));
				else  finalLiteral = new LiteralU(result);

				return new Token(expr.op->type, expr.op->lexeme, finalLiteral, (isInt_b) ? ValueType::INT : ValueType::DOUBLE, 0);
			}
			
			return new Token(expr.op->type, "Connected string", new LiteralU(finalStr), ValueType::STRING, 0);
		}
		throw  new RuntimeError(expr.op, "Operands must be numbers or strings.");
		return nullptr;
	
	}

	return NULL;
}



Token* Interpreter::visitCallExpr(CallExpr& expr) {

	Token* callee = evaluate(expr.callee);

	vector<Token*> arguments;
	for (Expr* argument : expr.arguments) {
		arguments.push_back(evaluate(argument));
	}



	if (callee->type != TokenType::CALLABLE && callee->type != TokenType::CLASS)
	{
		throw new RuntimeError(expr.paren, "Can only call functions and classes.");
	}

	ScriptCallable* function = callee->literal->callableValue;


	if (arguments.size() != function->arity()) {
		string s = "Expected " + to_string(function->arity()) + " arguments but got " + to_string(arguments.size()) + ".";
		throw  new RuntimeError(callee, s);
	}
	return function->call(this, arguments);
}
Token* Interpreter::visitConditionalExpr(ConditionalExpr& expr) {
	Token* condition = evaluate(expr.condition);
	Token* trueS = evaluate(expr.trueStmt);
	Token* falseS = evaluate(expr.falseStmt);
	
	return new Token(TokenType::NUMBER, "conditional", ((condition->literal->intValue) ? convertToLiteral(trueS) : convertToLiteral(falseS)), (condition->literal->intValue)?trueS->valueType:falseS->valueType, 0);
}
Token* Interpreter::visitGetExpr(GetExpr& expr) {

	Token* object = evaluate(expr.object);


	if (object->type == TokenType::INSTANCE)
	{
		return object->literal->instanceValue->get(expr.name);
	}

	throw  new RuntimeError(object, "Only instances have properties.");
}
Token* Interpreter::visitGroupingExpr(GroupingExpr& expr) {

	Token* token = evaluate(expr.expr);

	return token;
}
Token* Interpreter::visitLiteralExpr(LiteralExpr& expr) {


	return expr.value;
}

Token* Interpreter::visitLogicalExpr(LogicalExpr& expr) {


	Token* right = evaluate(expr.right);
	Token* left = evaluate(expr.left);

	if (expr.op->type == TokenType::LOGICAL_OR) {
		return new Token(TokenType::LOGICAL_OR, "logical or", new LiteralU(isTruthy(right) || isTruthy(left)), ValueType::BOOL,0);
	}
	else {
		return new Token(TokenType::LOGICAL_AND, "logical and", new LiteralU(isTruthy(right) && isTruthy(left)), ValueType::BOOL, 0);
	}

}
Token* Interpreter::visitSetExpr(SetExpr& expr) {

	Token* object = evaluate(expr.object);

	if (object->type != TokenType::INSTANCE) {
		throw  new RuntimeError(object, "Only instances have fields.");
	}

	Token* value = evaluate(expr.value);
	object->literal->instanceValue->set(expr.name, value);
	return value;
}
Token* Interpreter::visitSuperExpr(SuperExpr& expr) {
	int distance = locals[&expr];
	ScriptClass* superclass = environment->getAt(distance, "super")->literal->classValue;
	ScriptInstance* object = environment->getAt(distance - 1, "this")->literal->instanceValue;
	ScriptFunction* method = superclass->findMethod(expr.method->lexeme);
	if (method == nullptr) {
		throw  new RuntimeError(nullptr, "Undefined property '" + expr.method->lexeme + "'.");
	}

	return new Token(TokenType::CALLABLE, method->toString(), new LiteralU(method->bind(object)), ValueType::NONVALUE, 0); 
}
Token* Interpreter::visitThisExpr(ThisExpr& expr) {
	return lookUpVariable(expr.keyword, &expr);
}
Token* Interpreter::visitUnaryExpr(UnaryExpr& expr) {
	Token* right = evaluate(expr.value);

	Token* token;
	switch (expr.op->type) {
	case TokenType::BANG:
		token = new Token(TokenType::BANG, "Bang_Equal", new LiteralU(!isTruthy(right)), ValueType::BOOL, 0);
		return token;
	case TokenType::MINUS:
		if (right->valueType == ValueType::INT)
			return token = new Token(TokenType::MINUS, "minus", new LiteralU(-(right->literal->intValue)), ValueType::INT, 0);
		else 
			return token = new Token(TokenType::MINUS, "minus", new LiteralU(-(right->literal->doubleValue)), ValueType::DOUBLE, 0);

		
		return token;
	case TokenType::PLUSPLUS:
		if (expr.opIsLeft) {
			if (right->valueType == ValueType::INT)
				token = new Token(TokenType::PLUSPLUS, "Prefix", new LiteralU(++right->literal->intValue), right->valueType, 0);
			else
				token = new Token(TokenType::PLUSPLUS, "Prefix", new LiteralU(++right->literal->doubleValue), right->valueType, 0);
		}
		else {
			if(right->valueType == ValueType::INT)
				token = new Token(TokenType::PLUSPLUS, "Postfix", new LiteralU(right->literal->intValue++), right->valueType, 0);
			else
				token = new Token(TokenType::PLUSPLUS, "Postfix", new LiteralU(right->literal->doubleValue++), right->valueType, 0);
		}
		return token;
	case TokenType::MINUSMINUS:
		
		if (expr.opIsLeft) {
			if (right->valueType == ValueType::INT)
				token = new Token(TokenType::MINUSMINUS, "Prefix", new LiteralU(--right->literal->intValue), right->valueType, 0);
			else
				token = new Token(TokenType::MINUSMINUS, "Prefix", new LiteralU(--right->literal->doubleValue), right->valueType, 0);
		}
		else {
			if (right->valueType == ValueType::INT)
				token = new Token(TokenType::MINUSMINUS, "Postfix", new LiteralU(right->literal->intValue--), right->valueType, 0);
			else
				token = new Token(TokenType::MINUSMINUS, "Postfix", new LiteralU(right->literal->doubleValue--), right->valueType, 0);
		}
		return token;

	}

	// Unreachable.
	return NULL;
}
Token* Interpreter::visitVariableExpr(VariableExpr& expr) {
	return lookUpVariable(expr.token, &expr);
}
Token* Interpreter::lookUpVariable(Token* name, Expr* expr) {
	if (locals.find(expr) != locals.end()) {
		int distance = locals[expr];
		return environment->getAt(distance, name->lexeme);
	}
	else {
		return globals->get(name);
	}
}

//Stmt visitos
void Interpreter::visitBlockStmt(BlockStmt& stmt) {
	Environment* newEnv = new Environment(environment);
	executeBlock(stmt.statements, newEnv);
}
void Interpreter::visitBreakStmt(BreakStmt& stmt) {
	breakActivated = true;
}
void Interpreter::visitClassStmt(ClassStmt& stmt) {
	Token* superclass = nullptr;
	if (stmt.superclass != nullptr) {

		superclass = evaluate(stmt.superclass);
	
		if (superclass == nullptr || superclass->type != TokenType::CLASS) { 
			throw  new RuntimeError(superclass, "Superclass must be a class.");
		}
	}
	environment->define(stmt.name->lexeme, nullptr);


	if (stmt.superclass != nullptr) {
		environment = new Environment(environment);
		environment->define("super", superclass);
	}
	map<string, ScriptFunction*> methods;
	for (FunctionStmt* method : stmt.methods) {
		bool isInitializer = (method->name->lexeme == "init");
		ScriptFunction* function = new ScriptFunction(method, environment, (method->name->lexeme == "init"));
		methods[method->name->lexeme] = function;
	}
	ScriptClass* superclassValue = (superclass == nullptr) ? nullptr : superclass->literal->classValue;
	ScriptClass* klass = new ScriptClass(stmt.name->lexeme,
		(superclass == nullptr) ? nullptr : superclass->literal->classValue, methods);
	if (superclass != nullptr) {
		environment = environment->getEnclosing();
	}

	Token* token = new Token(TokenType::CLASS, klass->name, new LiteralU(klass), ValueType::NONVALUE, 0);
	environment->assign(stmt.name, token);

}
void Interpreter::visitContinueStmt(ContinueStmt& stmt) {
	continueActivated = true;
}
void Interpreter::visitDoWhileStmt(DoWhileStmt& stmt) {

	do {
		execute(stmt.body);
	} while (!breakActivated && isTruthy(evaluate(stmt.condition)));

	breakActivated = false;
	continueActivated = false;
}
void Interpreter::visitExpressionStmt(ExpressionStmt& stmt) {
	evaluate(stmt.expr);
}
void Interpreter::visitFunctionStmt(FunctionStmt& stmt) {
	ScriptFunction* function = new ScriptFunction(&stmt, environment, false);
	Token* token = new Token(TokenType::CALLABLE, function->toString(), new LiteralU(function), ValueType::NONVALUE, 0);
	environment->define(stmt.name->lexeme, token);
}
void Interpreter::visitIfStmt(IfStmt& stmt) {


	if (isTruthy(evaluate(stmt.condition))) {

		execute(stmt.thenBranch);

	}
	else if (stmt.elseBranch != NULL) {

		execute(stmt.elseBranch);

	}
}
void Interpreter::visitPrintStmt(PrintStmt& stmt) {

	Token* value = evaluate(stmt.expr);

	if (value != nullptr && value->type == TokenType::NEW_LINE)
		cout << endl;
	else 
		cout << ((value != nullptr) ? stringify(value) : "nil") << ((stmt.newLine)?"\n" : "");
}
bool Interpreter::isInteger(string& str) {
	size_t found = str.find("\"");

	if (found != std::string::npos) {
		return false;
	}
	try {
		size_t pos;
		std::stoi(str, &pos);
		return pos == str.size();  // Ensure the entire string was converted
	}
	catch (const std::exception& e) {
		return false;
	}
}

void Interpreter::visitReadStmt(ReadStmt& stmt) {
	std::string input;
	std::getline(std::cin, input);

	std::vector<std::string> values;
	std::istringstream iss(input);
	std::string value;

	while (std::getline(iss, value, ',')) {
		values.push_back(value);
	}

	if (values.size() != stmt.variables.size()) {
		throw new RuntimeError(nullptr, "Incorrect number of values in read statement.");
	}

	int valueIndex = 0;

	for (Token* variable : stmt.variables) {
		Token* token = nullptr;
	    std::string& stringValue = values[valueIndex++];

		if (stringValue == "true") {
			token = new Token(TokenType::TRUE, "TRUE", new LiteralU(1), ValueType::BOOL, 0);
		}
		else if (stringValue == "false") {
			token = new Token(TokenType::FALSE, "FALSE", new LiteralU(0), ValueType::BOOL, 0);
		}
		else {
			try {
				size_t pos;
				double doubleValue = std::stod(stringValue, &pos);
				if (pos == stringValue.size()) {
					bool isInt_ = std::trunc(doubleValue) == doubleValue;

					LiteralU* lit;

					if (isInt_)	lit = new LiteralU(static_cast<int>(doubleValue));
					else lit = new LiteralU(doubleValue);

					token = new Token(TokenType::NUMBER, "NUMBER", lit, (isInt_) ? ValueType::INT : ValueType::DOUBLE, 0);
				}
				else {
					token = new Token(TokenType::STRING, "STRING", new LiteralU(stringValue), ValueType::STRING, 0);
				}
			}
			catch (const std::exception& e) {
				token = new Token(TokenType::STRING, "STRING", new LiteralU(stringValue), ValueType::STRING, 0);
			}
		}

		if (token) {
			environment->define(variable->lexeme, token);
		}
	}
}

void Interpreter::visitReturnStmt(ReturnStmt& stmt) {
	Token* value = NULL;

	if (stmt.value != NULL) value = evaluate(stmt.value);

	throw new ReturnV(value);
}
void Interpreter::visitVarStmt(VarStmt& stmt) {
	Token* value = nullptr;


	if (stmt.initializer != nullptr) {

		value = evaluate(stmt.initializer);

	}

	environment->define(stmt.name->lexeme, value);
}
void Interpreter::visitWhileStmt(WhileStmt& stmt) {

	while (!breakActivated && isTruthy(evaluate(stmt.condition))) {
		execute(stmt.body);

	}


	
	breakActivated = false;
	continueActivated = false;
}
