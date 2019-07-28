#include "Parser.h"

#include <iostream>

Parser::Parser()
{}

void Parser::Parse(Lexer* lexer)
{
	for (Ty_uint32_t i = 0; i < lexer->GetTokens().size(); i++)
	{
		std::vector<Token> tokens;
		int j = 0;
		while (lexer->GetTokens()[i + j].Type != TokenType::END)
		{
			if (!lexer->GetTokens()[i + j].Type == TokenType::START)
				tokens.push_back(lexer->GetTokens()[i + j]);
			i++;
		}
		tokens.push_back(lexer->GetTokens()[i + j]);
		Grammar grammar = MatchGrammar(tokens);
		std::cout << grammar.Format << std::endl;

	}
}
