#ifndef Ty_PARSER_H
#define Ty_PARSER_H

#include "../Port.h"
#include "Lexer.h"
#include "Grammar.h"
#include "AST.h"

class Parser
{
public:
	Parser();
	
	void Parse(Lexer *lexer);
private:
	AST ProgramAST;
};

#endif