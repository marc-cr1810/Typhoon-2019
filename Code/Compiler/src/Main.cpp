#include <iostream>
#include "Typhoon.h"

#include "Parser/Lexer.h"

int main()
{
	File programFile("./examples/helloworld.ty");
	std::cout << "Compiling Program" << std::endl;

	Lexer lexer;
	lexer.TokenizeFile(programFile);
	for (Token token : lexer.GetTokens())
		std::cout << "Type: " << token.Type << " Value: " + token.Value << std::endl;

	std::cout << "Finished Compiling Program" << std::endl;
}