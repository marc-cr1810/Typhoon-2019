#include "Compiler.h"

Compiler::Compiler()
{}

void Compiler::Compile(Parser* parser)
{
	CompileASTNode(parser->GetAST().Program);
}

void Compiler::CompileASTNode(Node ast, int level)
{
	for (int i = 0; i < ast.Children.size(); i++)
	{
		Node node = ast.Children[i];

		if (node.Type == NodeType::NODE_STATEMENT)
		{
			if (node.StmtType == StatementType::FUNCTION_DECLARATION)
			{
				Ty_string_t label = "F_" + std::to_string(m_Linker.GetFunctions().size());
				Ty_string_t name = node.Value;
				std::vector<Ty_string_t> args;

				if (node.Children.size() > 1)
				{
					for (Node arg : node.Children[0].Children)
					{
						Ty_string_t label = "VA_" + std::to_string(m_Linker.GetVariables().size());
						Ty_string_t name = arg.Children[0].Value;
						if (arg.StmtType != StatementType::ASSIGN_NEW)
							return;
						args.push_back(name);
						m_Linker.AddVariable(name, label, AccessType::LOCAL, level + 1);
					}
				}

				m_Linker.AddFunction(name, label, args, level == 0 ? AccessType::GLOBAL : AccessType::LOCAL, level);

				int startPoint = m_Instructions.size();
				CompileASTNode(node.Children[args.size() > 0 ? 1 : 0], level + 1);
				if (m_Instructions.size() > startPoint)
					m_Instructions[startPoint].Label = label;
				
				std::cout << label << ": " << name << std::endl;
			}
			else if (node.StmtType == StatementType::IF)
			{

			}
			else if (node.StmtType == StatementType::ASSIGN_NEW)
			{
				Ty_string_t label = (level == 0 ? "VG_" : "VL_") + std::to_string(m_Linker.GetVariables().size());
				Ty_string_t name = node.Children[0].Value;
				m_Linker.AddVariable(name, label, level == 0 ? AccessType::GLOBAL : AccessType::LOCAL, level);
				if (node.Children.size() > 1)
					CompileASTNode(node.Children[1], level);
				else
					AddInstruction("", Bytecode::B_LDNULL);
				AddInstruction(label, level == 0 ? Bytecode::B_STORE : Bytecode::B_STLOC);
			}
			else if (node.StmtType == StatementType::EXPRESSION)
			{
				Node expression = node.Children[0];
				std::cout << node.Value << std::endl;
			}
		}
		else if (node.Type == NodeType::NODE_OBJECT)
		{
			switch (node.ObjType)
			{
			case ObjectType::OBJ_NAME:
				break;
			case ObjectType::OBJ_NUMBER:
				break;
			case ObjectType::OBJ_FLOAT:
				break;
			case ObjectType::OBJ_STRING:
				break;
			case ObjectType::OBJ_BOOL:
				break;
			case ObjectType::OBJ_FUNCTION_CALL:
				break;
			}
		}
	}
}

void Compiler::AddInstruction(Ty_string_t label, Bytecode opcode, std::vector<Ty_uint8_t> bytes)
{
	m_Instructions.push_back({ label, opcode, bytes });
}
