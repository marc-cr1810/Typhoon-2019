#include "Parser.h"

#include <iostream>

Parser::Parser()
{}

void Parser::Parse(Lexer* lexer)
{
	for (Ty_uint32_t i = 0; i < lexer->GetTokens().size(); i++)
	{
		std::vector<Token> tokens;
		Ty_uint32_t j = 0;
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
			Statement stmt(Statement::StatementType::ASSIGN, &ProgramAST.Program);
			TyObject varName(&stmt);

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
							varName.ObjType = TyObject::ObjectType::NAME;
							varName.Value = tokens[i].Value;
							i++;
							continue;
						}
					}
					else if (keyword == "EXPR")
					{
						std::vector<Token> exprTokens;
						while (lexer->GetTokens()[i].Type != TokenType::END)
						{
							exprTokens.push_back(lexer->GetTokens()[i]);
							i++;
						}
						Expression expr = ExpressionTokensToAST(exprTokens, &ProgramAST.Program);
					}
				}
			}
			break;
		}
	}
}

Expression Parser::ExpressionTokensToAST(std::vector<Token> tokens, Node* parent)
{
	std::stack<Token> stack;
	std::stack<Token> outputStack;

	for (Token token : tokens)
	{
		if (token.Type == TokenType::OPERATOR)
		{
			OperatorToken opToken = TokenToOperatorToken(token);
			if (opToken.OpType == OperatorType::LEFT_BRACKET)
				stack.push(token);
			else if (opToken.OpType == OperatorType::RIGHT_BRACKET)
			{
				while (stack.size() > 0)
				{
					if (stack.top().Type == TokenType::OPERATOR)
					{
						if (TokenToOperatorToken(stack.top()).OpType == OperatorType::LEFT_BRACKET)
							break;
						outputStack.push(stack.top());
						stack.pop();
					}
				}
				stack.pop();
			}
			else
			{
				while (stack.size() > 0)
				{
					if (stack.top().Type == TokenType::OPERATOR)
					{
						if (TokenToOperatorToken(stack.top()).Category < opToken.Category || TokenToOperatorToken(stack.top()).OpType == OperatorType::LEFT_BRACKET)
							break;
						outputStack.push(stack.top());
						stack.pop();
					}
					else
						outputStack.push(token);
				}
				stack.push(token);
			}
		}
		else
			outputStack.push(token);
	}
	while (stack.size() > 0)
	{
		outputStack.push(stack.top());
		stack.pop();
	}

	return Expression(parent);
}
