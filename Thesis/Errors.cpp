#include <iostream>
#include "Errors.h"
void Errors::report(int line, std::string where, std::string msg) {
	std::cerr << " [line " << line << "] Error" << where + ": " << msg << std::endl;
	hadError = true;
}

void Errors::error(Token* token, std::string msg) {
	if (token->type == TokenType::END_OF_FILE) {
		report(token->line, " at end", msg);
	}
	else {
		report(token->line, " at '" + token->lexeme + "'", msg);
	}
}


void Errors::error(int line, std::string msg) {
	report(line, "", msg);
}

bool Errors::getErrorType() {
	return hadError;
}

void Errors::runtimeError(RuntimeError* error) {
	if(error->token!=nullptr)
		std::cerr << "[line " << error->token->line << "]: " << error->getMessage() << "  (" << error->token->lexeme << ")" << std::endl;
	else
		std::cerr <<  error->getMessage() << std::endl;
	hadRuntimeError = true;
}
