#include "Parser.h"

#include <iostream>

Parser::Parser()
{}

void Parser::Parse(Lexer* lexer)
{
	m_ProgramAST.Program = ParseTokens(lexer->GetTokens());

	if (m_ProgramAST.Program.Type != NodeType::NODE_BLOCK)
		std::cout << "Failed To Compile Program!" << std::endl;
}

Node Parser::ParseTokens(std::vector<Token> tokens)
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
						if (statement.StmtType == StatementType::FUNCTION_DECLARATION)
						{
							statement.Value = tokens[i++].Value;
							continue;
						}
						else
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
							if (i < tokens.size() - 1 && tokens[i].Type == TokenType::NAME)
							{
								if (TokenToOperatorToken(tokens[i + 1]).OpType == OperatorType::LEFT_BRACKET)
								{
									Ty_string_t funcCall = tokens[i].Value + tokens[i + 1].Value + " ";
									int level = 1;
									i += 2;
									while (level > 0)
									{
										if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::LEFT_BRACKET)
										{
											level++;
										}
										else if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::RIGHT_BRACKET)
										{
											level--;
										}
										if (level > 0)
										{
											funcCall += tokens[i].Value + " ";
										}
										i++;
									}
									funcCall += ")";
									exprTokens.push_back({ TokenType::FUNCTION_CALL, funcCall });
								}
								else
									exprTokens.push_back(tokens[i++]);
							}
							else
								exprTokens.push_back(tokens[i++]);
						}
						statement.AddChild(ExpressionTokensToAST(exprTokens));
					}
					else if (keyword == "ARGS")
					{
						std::vector<Token> argTokens;

						int level = 1;
						i++;
						while (level > 0)
						{
							if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::LEFT_BRACKET)
							{
								level++;
							}
							else if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::RIGHT_BRACKET)
							{
								level--;
							}
							if (level > 0)
							{
								if (TokenToOperatorToken(tokens[i]).OpType == OperatorType::COMMA)
									argTokens.push_back({ TokenType::END, "\n" });
								else
									argTokens.push_back(tokens[i]);
							}
							i++;
						}

						if (argTokens.size() > 0)
						{
							argTokens.push_back({ TokenType::END, "\n" });
							statement.AddChild(ParseTokens(argTokens));
						}
					}
				}
			}

			if (statement.StmtType == StatementType::ELSE || statement.StmtType == StatementType::ELSE_IF)
			{
				if (block.Children[block.Children.size() - 1].Type == NodeType::NODE_STATEMENT)
				{
					if (block.Children[block.Children.size() - 1].StmtType == StatementType::IF)
					{
						Node* stmt = &block.Children[block.Children.size() - 1];
						int k = 0;
						while (k < stmt->Children.size())
						{
							if (stmt->Children[k].StmtType == StatementType::ELSE_IF)
							{
								stmt = &stmt->Children[k];
								k = 0;
							}
							else if (stmt->Children[k].StmtType == StatementType::ELSE)
							{
								std::cout << "Error: Unexpected else statement!" << std::endl;
								return Node();
							}
							k++;
						}
						stmt->AddChild(statement);
					}
					else
					{
						std::cout << "Error: expected a statement!" << std::endl;
						return Node();
					}
				}
				else
				{
					std::cout << "Error: expected a statement!" << std::endl;
					return Node();
				}
			}
			else
				block.AddChild(statement);
		}
		else
		{
			std::vector<Token> newBlock;
			while (tokens[i].Value.length() > 0 && tokens[i].Type == TokenType::START)
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
						if (stmt->Children[k].StmtType == StatementType::ELSE_IF)
						{
							stmt = &stmt->Children[k];
							k = 0;
						}
						else if (stmt->Children[k].StmtType == StatementType::ELSE)
							stmt = &stmt->Children[k];
						k++;
					}
					stmt->AddChild(ParseTokens(newBlock));
				}
				else
					block.Children[block.Children.size() - 1].AddChild(ParseTokens(newBlock));
			}
			else
				block.AddChild(ParseTokens(newBlock));
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

	return RPNToAST(&outputStack);
}

Node Parser::RPNToAST(std::stack<Token>* stack, OperatorType opType)
{
	Node expr;
	expr.Type = NodeType::NODE_EXPRESSION;
	expr.OpType = opType;

	if (stack->size() > 1)
	{
		if (opType == OperatorType::UNKNOWN)
		{
			Token op = stack->top();
			stack->pop();
			if (op.Type != TokenType::OPERATOR)
			{
				std::cout << "Warning: Expected an operator!" << std::endl;
				expr.AddChild(NewObjectNode((ObjectType)op.Type, op.Value));
			}
			else
				expr.OpType = TokenToOperatorToken(op).OpType;
		}

		Token rightToken = stack->top();
		stack->pop();
		Node right = GetRPNNodeFromToken(stack, rightToken);

		Token leftToken = stack->top();
		stack->pop();
		Node left = GetRPNNodeFromToken(stack, leftToken);

		expr.AddChild(right);
		expr.AddChild(left);
	}
	else if (stack->size() == 1)
	{
		Token token = stack->top();
		stack->pop();
		expr = GetRPNNodeFromToken(stack, token);
	}

	return expr;
}

Node Parser::GetRPNNodeFromToken(std::stack<Token>* stack, Token token)
{
	Node node;

	if (token.Type == TokenType::OPERATOR)
		node = RPNToAST(stack, TokenToOperatorToken(token).OpType);
	else if (token.Type == TokenType::FUNCTION_CALL)
	{
		Lexer lexer;
		lexer.TokenizeString(token.Value);

		node = NewObjectNode(ObjectType::OBJ_FUNCTION_CALL, lexer.GetTokens()[0].Value);

		std::vector<Token> argTokens;

		int level = 1;
		int i = 2;
		while (level > 0)
		{
			if (TokenToOperatorToken(lexer.GetTokens()[i]).OpType == OperatorType::LEFT_BRACKET)
			{
				level++;
			}
			else if (TokenToOperatorToken(lexer.GetTokens()[i]).OpType == OperatorType::RIGHT_BRACKET)
			{
				level--;
			}
			if (level > 0)
			{
				if (TokenToOperatorToken(lexer.GetTokens()[i]).OpType == OperatorType::COMMA)
					argTokens.push_back({ TokenType::END, "\n" });
				else if (lexer.GetTokens()[i].Type == TokenType::STRING)
					argTokens.push_back({ TokenType::STRING, std::regex_replace(lexer.GetTokens()[i].Value, std::regex("\\\\n"), "\n") });
				else
					argTokens.push_back(lexer.GetTokens()[i]);
			}
			i++;
		}
		if (argTokens.size() > 0)
			argTokens.push_back({ TokenType::END, "\n" });
		node.AddChild(ParseTokens(argTokens));
	}
	else
		node = NewObjectNode((ObjectType)token.Type, token.Value);

	return node;
}
