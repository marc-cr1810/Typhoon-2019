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

	AST GetAST() const { return m_ProgramAST; }
private:
	Node ParseTokens(std::vector<Token> tokens);
	Node ExpressionTokensToAST(std::vector<Token> tokens);
	Node RPNToAST(std::stack<Token>* stack, OperatorType opType = OperatorType::UNKNOWN);
	Node GetRPNNodeFromToken(std::stack<Token>* stack, Token token);

	AST m_ProgramAST;
};

#endif