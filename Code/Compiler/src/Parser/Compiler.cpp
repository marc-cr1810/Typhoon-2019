#include "Compiler.h"

Compiler::Compiler()
{}

void Compiler::Compile(AST ast)
{
	for (int i = 0; i < ast.Program.Children.size(); i++)
	{
		Node node = ast.Program.Children[i];

		if (node.Type == NodeType::NODE_STATEMENT)
		{
			if (node.StmtType == StatementType::FUNCTION_DECLARATION)
			{

			}
			else if (node.StmtType == StatementType::IF)
			{

			}
			else if (node.StmtType == StatementType::ASSIGN_NEW)
			{

			}
			else if (node.StmtType == StatementType::EXPRESSION)
			{

			}
		}
	}
}
