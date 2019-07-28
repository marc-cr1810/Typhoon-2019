#ifndef Ty_GRAMMAR_H
#define Ty_GRAMMAR_H

#include "../Port.h"
#include "Token.h"

enum GrammarType
{
	UNKNOWN_GRAMMAR = -1,
	CREATE_VAR,
	SET_VAR,
	IF_STATEMENT,
	ELSE_STATEMENT
};

enum GrammarKeywordType
{
	UNKNOWN_KEYWORD = -1,
	KW_NAME,
	KW_EXPR
};

struct Grammar
{
	GrammarType Type;
	Ty_string_t Format;

	Grammar(GrammarType type, Ty_string_t format)
		: Type(type), Format(format)
	{}
};

const Ty_int32_t ExpressionTokenTypes[6] = {
	TokenType::NAME, 
	TokenType::NUMBER, 
	TokenType::FLOAT, 
	TokenType::STRING, 
	TokenType::BOOL,
	TokenType::OPERATOR
 };

const Grammar GrammarFormats[4] = {
	{ GrammarType::CREATE_VAR, "'var' NAME '=' EXPR" },
	{ GrammarType::SET_VAR, "NAME '=' EXPR" },
	{ GrammarType::IF_STATEMENT, "'if' EXPR ':'" },
	{ GrammarType::ELSE_STATEMENT, "'else'" }
};

static std::vector<Ty_string_t> GetGrammarKeywordList(Grammar grammar)
{
	Ty_string_t s = grammar.Format;
	std::smatch m;
	std::regex r("[^\\s\']+|\'([^\']*)\'");
	std::vector<Ty_string_t> result;

	while (std::regex_search(s, m, r))
	{
		result.push_back(m[0]);
		s = m.suffix().str();
	}
	return result;
}

#include <iostream>

static Grammar MatchGrammar(std::vector<Token> tokens)
{
	for (Grammar grammar : GrammarFormats)
	{
		int i = 0;
		std::vector<Ty_string_t> keywords = GetGrammarKeywordList(grammar);
		for (Ty_string_t keyword : keywords)
		{
			std::smatch m;
			if (std::regex_match(keyword, m, std::regex("^\'([^\']+)\'$")))
			{
				if (m[1] == tokens[i].Value)
				{
					std::cout << keyword << ", ";
					i++;
					continue;
				}
				else goto NextFormat;
			}
			else
			{
				std::cout << keyword << ", ";
				if (keyword == "NAME")
					i++;
				else if (keyword == "EXPR")
				{
					bool isExpression = false;
					for (Ty_int32_t exprTokenType : ExpressionTokenTypes)
					{
						if (tokens[i].Type == exprTokenType)
						{
							isExpression = true;
							i++;
						}
						else break;
					}
					if (!isExpression)
						goto NextFormat;
				}
			}
		}
	NextFormat: ;
	}
	return { GrammarType::UNKNOWN_GRAMMAR, "" };
}

#endif