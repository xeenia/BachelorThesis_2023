
#include "Token.h"
#include "ScriptFunction.h"

LiteralU::LiteralU()
{
	std::memset(this, 0, sizeof(LiteralU));
}

LiteralU::LiteralU(double _doubleValue) : LiteralU()
{
	doubleValue = _doubleValue;
}

LiteralU::LiteralU(int _intValue) : LiteralU()
{
	intValue = _intValue;
}

LiteralU::LiteralU(ScriptClass* _classValue) : LiteralU()
{
	classValue = _classValue;
}

LiteralU::LiteralU(ScriptInstance* _instanceValue) : LiteralU()
{
	instanceValue = _instanceValue;
}

LiteralU::LiteralU(void* _nullValue) : LiteralU()
{
	nullValue = _nullValue;
}

LiteralU::LiteralU(string& _stringValue) : LiteralU()
{
	stringValue = new char[_stringValue.size() + 1];
	strcpy_s(stringValue, _stringValue.size() + 1, _stringValue.c_str());
}

LiteralU::LiteralU(ScriptCallable* _callableValue) : LiteralU()
{
	callableValue = _callableValue;
}
Token::Token(TokenType type, std::string lexeme, LiteralU* literal, ValueType valueType, int line) {
	this->type = type;
	this->lexeme = lexeme;
	this->line = line + 1;
	this->literal = literal;
	this->valueType = valueType;
}



