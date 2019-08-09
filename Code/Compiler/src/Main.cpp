#include <iostream>
#include "Typhoon.h"

#include "Parser/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/Compiler.h"

int main()
{
	File programFile("./examples/helloworld.ty");
	std::cout << "Compiling Program" << std::endl;

	Lexer lexer;
	lexer.TokenizeFile(programFile);

	Parser parser;
	parser.Parse(&lexer);

	Compiler compiler;
	compiler.Compile(&parser);

	std::cout << "Finished Compiling Program" << std::endl;
}