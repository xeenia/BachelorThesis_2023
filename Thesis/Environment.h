#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "Token.h"
#include <string>
using namespace std;

class Environment {	
public:
   

	Environment();
	Environment(Environment* enclosing);
	 
	void define(string name, Token* value); 
	Token* get(Token* name);
	Token* getAt(int distance, string name);
	Environment* ancestor(int distance);
	void assign(Token* name, Token* value);
	void assignAt(int distance, Token* name, Token* value);
	Environment* getEnclosing();
private:
	map<string, Token*> values;
	Environment* enclosing;
};
#endif  // ENVIRONMENT_H