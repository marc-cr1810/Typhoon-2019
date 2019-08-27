#include "Typhoon.h"

#include "Parser/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/Compiler.h"

int main(int argc, char** argv)
{
	Ty_string_t programFilePath = "";
	Ty_string_t outputFilePath = "";
	for (int i = 1; i < argc; ++i)
	{
		if (Ty_string_t(argv[i]) == "-o")
			outputFilePath = Ty_string_t(argv[++i]);
		else
			programFilePath = Ty_string_t(argv[i]);
	}

	File programFile(programFilePath.c_str());
	std::cout << "Compiling Program" << std::endl;

	Lexer lexer;
	lexer.TokenizeFile(programFile);

	Parser parser;
	parser.Parse(&lexer);

	Compiler compiler(outputFilePath);
	compiler.Compile(&parser);

	std::cout << "Finished Compiling Program" << std::endl;
}