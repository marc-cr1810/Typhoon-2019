#include <iostream>
#include "Typhoon.h"

#include "Parser/Lexer.h"
#include "Parser/Parser.h"

int main()
{
	File programFile("./examples/helloworld.ty");
	std::cout << "Compiling Program" << std::endl;

	Lexer lexer;
	lexer.TokenizeFile(programFile);

	Parser parser;
	parser.Parse(&lexer);

	std::cout << "Finished Compiling Program" << std::endl;
}