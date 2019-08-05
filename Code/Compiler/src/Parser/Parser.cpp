#include "Parser.h"

#include <iostream>

Parser::Parser()
{}

void Parser::Parse(Lexer* lexer)
{
	ProgramAST.Program = ParseTokens(lexer->GetTokens());
}

Node Parser::ParseTokens(std::vector<Token> tokens, int level)
{
	Node block;
	block.Type = NodeType::NODE_BLOCK;

	for (Ty_uint32_t i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].Type != TokenType::START)
		{
			std::vector<Token> grammarTokens;
			Ty_uint32_t j = 0;
			while (tokens[i + j].Type != TokenType::END)
			{
				if (!tokens[i + j].Type == TokenType::START)
					grammarTokens.push_back(tokens[i + j]);
				j++;
			}
			grammarTokens.push_back(tokens[i + j]);
			Grammar grammar = MatchGrammar(grammarTokens);

			Node statement = NewStatementNode((StatementType)grammar.Type);

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
							statement.AddChild(NewObjectNode(ObjectType::OBJ_NAME, tokens[i++].Value));
							continue;
						}
					}
					else if (keyword == "EXPR")
					{
						std::vector<Token> exprTokens;
						while (tokens[i].Type != TokenType::END && tokens[i].Type != TokenType::OPERATOR_SPECIAL)
						{
							exprTokens.push_back(tokens[i++]);
						}
						statement.AddChild(ExpressionTokensToAST(exprTokens));
					}
				}
			}

			if (statement.StmtType == StatementType::ELSE)
			{
				if (block.Children[block.Children.size() - 1].Type == NodeType::NODE_STATEMENT)
				{
					if (block.Children[block.Children.size() - 1].StmtType == StatementType::IF)
						block.Children[block.Children.size() - 1].AddChild(statement);
					else
					{
						std::cout << "Error: expected a statement before else!" << std::endl;
						return Node();
					}
				}
			}
			else
				block.AddChild(statement);
		}
		else
		{
			std::vector<Token> newBlock;
			while (tokens[i].Value.length() > level && tokens[i].Type == TokenType::START)
			{
				tokens[i].Value = tokens[i].Value.substr(0, tokens[i].Value.length() - 1);
				if (tokens[i].Value.length() > 0)
					newBlock.push_back(tokens[i++]);
				else
					i++;

				while (tokens[i].Type != TokenType::END)
					newBlock.push_back(tokens[i++]);
				newBlock.push_back(tokens[i++]);

				if (i >= tokens.size())
					break;
			}

			if (block.Children[block.Children.size() - 1].Type == NodeType::NODE_STATEMENT)
			{
				if (block.Children[block.Children.size() - 1].StmtType == StatementType::IF)
				{
					Node* stmt = &block.Children[block.Children.size() - 1];
					int k = 0;
					while (k < stmt->Children.size())
					{
						if (stmt->Children[k].StmtType == StatementType::IF)
						{
							stmt = &stmt->Children[k];
							k = 0;
						}
						else if (stmt->Children[k].StmtType == StatementType::ELSE)
							stmt = &stmt->Children[k];
						k++;
					}
					stmt->AddChild(ParseTokens(newBlock, level + 1));
				}
				else
					block.Children[block.Children.size() - 1].AddChild(ParseTokens(newBlock, level + 1));
			}
			else
				block.AddChild(ParseTokens(newBlock, level + 1));
			i--;
		}
	}
	return block;
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
				expr.OpType = TokenToOperatorToken(op).OpType;
		}

		Token right = stack.top();
		stack.pop();
		Token left = stack.top();
		stack.pop();

		if (left.Type != TokenType::OPERATOR)
			expr.AddChild(NewObjectNode((ObjectType)left.Type, left.Value));
		else
			expr.AddChild(RPNToAST(stack, TokenToOperatorToken(left).OpType));

		if (right.Type != TokenType::OPERATOR)
			expr.AddChild(NewObjectNode((ObjectType)right.Type, right.Value));
		else
			expr.AddChild(RPNToAST(stack, TokenToOperatorToken(right).OpType));
	}
	else if (stack.size() == 1)
	{
		Token token = stack.top();
		stack.pop();
		expr = NewObjectNode((ObjectType)token.Type, token.Value);
	}

	return expr;
}
