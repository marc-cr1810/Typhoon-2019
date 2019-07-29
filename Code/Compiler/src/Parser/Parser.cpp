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
			j++;
		}
		tokens.push_back(lexer->GetTokens()[i + j]);
		Grammar grammar = MatchGrammar(tokens);
		std::cout << grammar.Format << std::endl;

		switch (grammar.Type)
		{
		case GrammarType::CREATE_VAR:
			Statement stmt(Statement::StatementType::ASSIGN, &ProgramAST.Program); ProgramAST.Program.Add(stmt);
			TyObject varName(&stmt); stmt.Add(&varName);
			Expression expr(&stmt); stmt.Add(&expr);

			std::vector<Ty_string_t> keywords = GetGrammarKeywordList(grammar);
			for (Ty_string_t keyword : keywords)
			{
				std::smatch m;
				if (std::regex_match(keyword, m, std::regex("^\'([^\']+)\'$")))
				{
					if (m[1] == tokens[i].Value)
					{
						i++;
						continue;
					}
				}
				else
				{
					if (keyword == "NAME")
					{
						if (tokens[i].Type == TokenType::NAME)
						{
							varName.Type = TyObject::ObjectType::NAME;
							varName.Value = tokens[i].Value;
							i++;
							continue;
						}
					}
					else if (keyword == "EXPR")
					{
						while (lexer->GetTokens()[i].Type != TokenType::END)
						{
							i++;
						}
					}
				}
			}
			break;
		}
	}
}
