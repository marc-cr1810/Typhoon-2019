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

		Node statement = NewStatementNode(StatementType::UNKNOWN_STATEMENT);
		switch (grammar.Type)
		{
		case GrammarType::CREATE_VAR:
			statement.StmtType = StatementType::ASSIGN;
			break;
		}

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
						statement.AddChild(NewObjectNode(ObjectType::OBJ_NAME, tokens[i].Value));
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
					statement.AddChild(ExpressionTokensToAST(exprTokens));
				}
			}
		}

		ProgramAST.Program.AddChild(statement);
	}
}

Node Parser::ExpressionTokensToAST(std::vector<Token> tokens)
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

	return RPNToAST(outputStack);
}

Node Parser::RPNToAST(std::stack<Token> stack, OperatorType opType)
{
	Node expr;
	expr.Type = NodeType::NODE_EXPRESSION;
	expr.OpType = opType;

	if (stack.size() > 1)
	{
		if (opType == OperatorType::UNKNOWN)
		{
			Token op = stack.top();
			stack.pop();
			if (op.Type != TokenType::OPERATOR)
			{
				std::cout << "Warning: Expected an operator!" << std::endl;
				expr.AddChild(NewObjectNode((ObjectType)op.Type, op.Value));
			}
			else
			{
				expr.OpType = TokenToOperatorToken(op).OpType;
			}
		}

		Token right = stack.top();
		stack.pop();
		Token left = stack.top();
		stack.pop();

		if (left.Type != TokenType::OPERATOR)
		{
			expr.AddChild(NewObjectNode((ObjectType)left.Type, left.Value));
		}
		else
		{
			expr.AddChild(RPNToAST(stack, TokenToOperatorToken(left).OpType));
		}

		if (right.Type != TokenType::OPERATOR)
		{
			expr.AddChild(NewObjectNode((ObjectType)right.Type, right.Value));
		}
		else
		{
			expr.AddChild(RPNToAST(stack, TokenToOperatorToken(right).OpType));
		}
	}
	else if (stack.size() == 1)
	{
		Token token = stack.top();
		stack.pop();
		expr = NewObjectNode((ObjectType)token.Type, token.Value);
	}

	return expr;
}
