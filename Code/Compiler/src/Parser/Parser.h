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
	Node ParseTokens(std::vector<Token> tokens);
	Node ExpressionTokensToAST(std::vector<Token> tokens);
	Node RPNToAST(std::stack<Token>* stack, OperatorType opType = OperatorType::UNKNOWN);

	AST ProgramAST;
};

#endif