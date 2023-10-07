#include <iostream>
#include "RuntimeError.h"

RuntimeError::RuntimeError(Token* token, const std::string& message)
    : std::runtime_error(message.c_str()), token(token), message(message.c_str()) {
}

string RuntimeError::getMessage() {
	return message;
}