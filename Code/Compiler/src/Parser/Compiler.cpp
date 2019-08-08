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
				Ty_string_t label = "F_" + std::to_string(m_Linker.GetFunctions().size());
				Ty_string_t name = node.Value;
				int argCount = node.Children.size() > 1 ? node.Children[0].Children.size() : 0;

				if (argCount > 0)
				{
					for (Node arg : node.Children[0].Children)
					{
						if (arg.StmtType != StatementType::ASSIGN_NEW)
							return;
					}
				}

				m_Linker.AddFunction(name, label, argCount);
				
				std::cout << label << ": " << name << std::endl;
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

void Compiler::AddInstruction(Instruction instruction)
{
	m_Instructions.push_back(instruction);
}
