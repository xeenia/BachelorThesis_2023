
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>
#include <cstdio>
#include <cstdlib>

#include "Expr.h"
#include "Errors.h"
#include "TokenType.h"
#include "Token.h"
#include "Resolver.h"

#include "Scanner.h"
#include "ASTPrinter.h"
#include "Parser.h"
#include "Interpreter.h"

#include <conio.h>

using namespace std;
Errors err;
static Interpreter interpreter(&err);
void run(string source) {


	Scanner scanner(source, &err);
	vector<Token*> tokens = scanner.scanTokens();
	
	
	Parser parser(tokens, &err);
	list<Stmt*> statements = parser.parseStmt();
	
	// Stop if there was a syntax error.
	if (err.getErrorType()) return;


	ASTPrinter astPrinter;
	astPrinter.printer(statements);


	ofstream* ASTSyntaxTree = new ofstream("ASTSyntaxTree.dot", ofstream::out);
	(*ASTSyntaxTree) << "digraph G{\n node [shape=box, style=filled, fontname=\"Helvetica\", fontsize=10];\n";
	(*ASTSyntaxTree) << astPrinter.dotContentLabels;
	(*ASTSyntaxTree) << astPrinter.dotContentEdges;
	(*ASTSyntaxTree) << " }";

	ASTSyntaxTree->close();

	//system("dot -Tgif ASTSyntaxTree.dot -o ASTSyntaxTree.dot.gif");


	Resolver resolver(&interpreter, &err);
	resolver.resolve(statements);

	if (err.getErrorType()) return;
	
	interpreter.interpret(statements);

}

void runFile(string argv) {
	std::ifstream infile;
	infile.open(argv);
	string data;
	if (!infile.is_open()) {
		cerr << "Failed to open file. PATH: " << argv << endl;
		return;
	}

	std::string content((std::istreambuf_iterator<char>(infile)), (std::istreambuf_iterator<char>()));

	infile.close();
	run(content);
	if (err.getErrorType()) exit(0);
	if (err.hadRuntimeError) exit(70);
}


void intro() {
	for (int i=0; i < 5; i++) {
		for (int j = 0; j < 40; j++) {
			if (i == 0 || i == 4) {
				cout << "-";
			}
			else if(i == 1){
				cout << "\t" << "run project_name.txt";
				break;
			}
			else if (i == 2) {
				cout << "\t" << "run /path/project_name.txt";
				break;
			}
		}
		cout << endl;
	}
}


int main(int argc, char* argv[]) {
	
	string input="";

	if (argc == 1) {
		intro();
		cout << "\n\n> run  ";
		std::getline(std::cin, input);
		for (char ch : input) {
			if (ch == ' ') {
				input += ch;
			}
		}
		system("CLS");
	}
	else if(argc == 2){
		input = argv[1];
	}
	else {
		cout << "Error: wrong number of arguments." << endl;
		return 0;
	}

	runFile(input);
	
	return 0;
}