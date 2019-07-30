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
	Expression ExpressionTokensToAST(std::vector<Token> tokens, Node* parent);
	Expression RPNToAST(std::stack<Token> stack, Node* parent);

	AST ProgramAST;
};

#endif