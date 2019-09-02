#include "Compiler.h"

Compiler::Compiler(Ty_string_t output)
	: m_OutputPath(output)
{}

void Compiler::Compile(Parser* parser)
{
	CompileASTNode(parser->GetAST().Program);
	m_Linker.Link(&m_Instructions);
	BuildMachineCode();
}

void Compiler::CompileASTBlock(Node block, int scope)
{
	CompileASTNode(block, scope);
	m_Linker.RemoveFunctions(scope);
	m_Linker.RemoveVariables(scope);
}

void Compiler::CompileASTNode(Node ast, int scope)
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
						m_Linker.AddVariable(name, label, AccessType::LOCAL, scope + 1);
					}
				}

				m_Linker.AddFunction(name, label, args, scope == 0 ? AccessType::GLOBAL : AccessType::LOCAL, scope);

				Branch endPoint = *m_Linker.AddBranch();
				AddInstruction("", Bytecode::B_BR, StringToVector(endPoint.Name));

				int startPoint = m_Instructions.size();
				CompileASTBlock(node.Children[args.size() > 0 ? 1 : 0], scope + 1);
				AddInstruction("", Bytecode::B_RET);

				AddInstruction(endPoint.Name, Bytecode::B_NOP);
				m_Instructions[startPoint].Label = label;
				
				std::cout << label << ": " << name << std::endl;
			}
			else if (node.StmtType == StatementType::IF)
			{
				Branch endPoint = *m_Linker.AddBranch();
				Branch nextPoint;
				CompileASTNode(node.Children[0], scope);

				if (node.Children.size() > 2)
				{
					nextPoint = *m_Linker.AddBranch();
					AddInstruction("", Bytecode::B_BRFALSE, StringToVector(nextPoint.Name));
					CompileASTBlock(node.Children[1], scope + 1);

					Node* n = &node.Children[2]; 
					while (n->Children.size() > 2)
					{
						AddInstruction("", Bytecode::B_BR, StringToVector(endPoint.Name));
						int nextIfStart = m_Instructions.size();
						CompileASTNode(n->Children[0], scope);
						m_Instructions[nextIfStart].Label = nextPoint.Name;
						nextPoint = *m_Linker.AddBranch();
						AddInstruction("", Bytecode::B_BRFALSE, StringToVector(nextPoint.Name));
						CompileASTBlock(n->Children[1], scope + 1);
						n = &n->Children[2];
					}

					if (n->StmtType == StatementType::ELSE_IF)
					{
						AddInstruction("", Bytecode::B_BR, StringToVector(endPoint.Name));
						int nextIfStart = m_Instructions.size();
						CompileASTNode(n->Children[0], scope);
						m_Instructions[nextIfStart].Label = nextPoint.Name;
						AddInstruction("", Bytecode::B_BRFALSE, StringToVector(endPoint.Name));
						CompileASTBlock(n->Children[1], scope + 1);
					}
					else if (n->StmtType == StatementType::ELSE)
					{
						AddInstruction("", Bytecode::B_BR, StringToVector(endPoint.Name));
						int elseStart = m_Instructions.size();
						CompileASTNode(n->Children[0], scope);
						m_Instructions[elseStart].Label = nextPoint.Name;
					}
				}
				else
				{
					AddInstruction("", Bytecode::B_BRFALSE, StringToVector(endPoint.Name));
					CompileASTBlock(node.Children[1], scope + 1);
				}

				AddInstruction(endPoint.Name, Bytecode::B_NOP);
				std::cout << node.Value << std::endl;
			}
			else if (node.StmtType == StatementType::ASSIGN_NEW)
			{
				Ty_string_t label = (scope == 0 ? "VG_" : "VL_") + std::to_string(m_Linker.GetVariables().size());
				Ty_string_t name = node.Children[0].Value;
				Variable var = *m_Linker.AddVariable(name, label, scope == 0 ? AccessType::GLOBAL : AccessType::LOCAL, scope);
				
				if (node.Children.size() > 1)
					CompileASTNode(node.Children[1], scope);
				else
					AddInstruction("", Bytecode::B_LDNULL);

				Bytecode bytecode = Bytecode::B_NOP;
				std::vector<Ty_uint8_t> bytes = IntToBytes(var.ID);
				if (bytes.size() == 1)
					bytecode = scope == 0 ? Bytecode::B_STORE_S : Bytecode::B_STLOC_S;
				else if (bytes.size() == 2)
					bytecode = scope == 0 ? Bytecode::B_STORE : Bytecode::B_STLOC;
				else if (bytes.size() == 4)
					bytecode = scope == 0 ? Bytecode::B_STORE_L : Bytecode::B_STLOC_L;
				AddInstruction("", bytecode, bytes);
				AddInstruction("", Bytecode::B_POP, std::vector<Ty_uint8_t>());
			}
			else if (node.StmtType == StatementType::EXPRESSION)
			{
				CompileASTNode(node.Children[0], scope);
				AddInstruction("", Bytecode::B_POP, std::vector<Ty_uint8_t>());
			}
		}
		else if (node.Type == NodeType::NODE_EXPRESSION)
		{
			CompileASTNode(node, scope);
		}
		else if (node.Type == NodeType::NODE_OBJECT)
		{
			CompileObject(node, scope);
		}
	}

	if (ast.Type == NodeType::NODE_EXPRESSION)
	{
		CompileExpression(ast, scope);
	}
	else if (ast.Type == NodeType::NODE_OBJECT)
	{
		CompileObject(ast, scope);
	}
}

void Compiler::CompileObject(Node object, int scope)
{
	if (object.Type == NodeType::NODE_OBJECT)
	{
		switch (object.ObjType)
		{
		case ObjectType::OBJ_NAME:
		{
			Variable var = *m_Linker.GetVariableFromName(object.Value);
			VariableType type = m_Linker.GetVarTypeFromLabel(var.LabelName);
			std::vector<Ty_uint8_t> bytes = IntToBytes(var.ID);
			if (bytes.size() == 1)
				AddInstruction("",
					type == VariableType::VAR_GLOBAL ? Bytecode::B_LOAD_S : 
					type == VariableType::VAR_LOCAL ? Bytecode::B_LDLOC_S : Bytecode::B_LDARG_S, bytes);
			else if (bytes.size() == 2)
				AddInstruction("",
					type == VariableType::VAR_GLOBAL ? Bytecode::B_LOAD :
					type == VariableType::VAR_LOCAL ? Bytecode::B_LDLOC : Bytecode::B_LDARG, bytes);
			else if (bytes.size() == 4)
				AddInstruction("",
					type == VariableType::VAR_GLOBAL ? Bytecode::B_LOAD_L :
					type == VariableType::VAR_LOCAL ? Bytecode::B_LDLOC_L : Bytecode::B_LDARG_L, bytes);
		}
			break;
		case ObjectType::OBJ_NUMBER:
		{
			int value = std::stoi(object.Value);
			std::vector<Ty_uint8_t> bytes = IntToBytes(value);
			if (bytes.size() == 1)
				AddInstruction("", Bytecode::B_LDCONST_S, bytes);
			else if (bytes.size() == 2)
				AddInstruction("", Bytecode::B_LDCONST, bytes);
			else if (bytes.size() == 4)
				AddInstruction("", Bytecode::B_LDCONST_L, bytes);
		}
		break;
		case ObjectType::OBJ_FLOAT:
		{
			float value = std::stof(object.Value);
			std::vector<Ty_uint8_t> bytes = FloatToBytes(value);
			AddInstruction("", Bytecode::B_LDFLOAT_S, bytes);
		}
			break;
		case ObjectType::OBJ_STRING:
		{
			Ty_string_t value = object.Value.substr(1, object.Value.length() - 2);
			std::vector<Ty_uint8_t> bytes = StringToVector(value);
			bytes.push_back('\0');
			AddInstruction("", Bytecode::B_LDSTR, bytes);
		}
			break;
		case ObjectType::OBJ_BOOL:
			if (object.Value == "true" || object.Value == "True")
				AddInstruction("", Bytecode::B_LDTRUE, std::vector<Ty_uint8_t>());
			else if (object.Value == "false" || object.Value == "False")
				AddInstruction("", Bytecode::B_LDFALSE, std::vector<Ty_uint8_t>());
			break;
		case ObjectType::OBJ_FUNCTION_CALL:
		{
			Function* function = m_Linker.GetFunctionFromNameArgCount(object.Value, object.Children[0].Children.size());
			if (!function->InBuilt)
			{
				for (int i = 0; i < object.Children[0].Children.size(); i++)
				{
					CompileASTNode(object.Children[0].Children[i], scope);
					std::vector<Ty_uint8_t> bytes = IntToBytes(i);
					if (bytes.size() == 1)
						AddInstruction("", Bytecode::B_LDARG_S, bytes);
					else if (bytes.size() == 2)
						AddInstruction("", Bytecode::B_LDARG, bytes);
					else if (bytes.size() == 4)
						AddInstruction("", Bytecode::B_LDARG_L, bytes);
				}
				AddInstruction("", Bytecode::B_CALL, StringToVector(function->LabelName));
			}
			else
			{
				if (function->LabelName == "F_SYSCALL")
				{
					Ty_uint16_t systemCallCode = std::stoi(object.Children[0].Children[0].Children[0].Value);

					for (int i = 1; i < object.Children[0].Children.size(); i++)
					{
						CompileASTNode(object.Children[0].Children[i], scope);
						std::vector<Ty_uint8_t> bytes = IntToBytes(i - 1);
						if (bytes.size() == 1)
							AddInstruction("", Bytecode::B_STARG_S, bytes);
						else if (bytes.size() == 2)
							AddInstruction("", Bytecode::B_STARG, bytes);
						else if (bytes.size() == 4)
							AddInstruction("", Bytecode::B_STARG_L, bytes);
					}
					AddInstruction("", Bytecode::B_SYSCALL, { (Ty_uint8_t)((systemCallCode >> 8) & 0xFF), (Ty_uint8_t)(systemCallCode & 0xFF) });
				}
			}
		}
			break;
		}
	}
}

void Compiler::CompileExpression(Node object, int scope)
{
	switch (object.OpType)
	{
	case OperatorType::ADD:
		AddInstruction("", Bytecode::B_ADD);
		break;
	case OperatorType::SUBTRACT:
		AddInstruction("", Bytecode::B_SUB);
		break;
	case OperatorType::MULTIPLY:
		AddInstruction("", Bytecode::B_MUL);
		break;
	case OperatorType::DIVIDE:
		AddInstruction("", Bytecode::B_DIV);
		break;
	case OperatorType::EQUAL:
	{
		Instruction* instruction = &m_Instructions[m_Instructions.size() - 1];
		switch (instruction->Opcode)
		{
		case Bytecode::B_LOAD_S:
			instruction->Opcode = Bytecode::B_STORE_S;
			break;
		case Bytecode::B_LDLOC_S:
			instruction->Opcode = Bytecode::B_STLOC_S;
			break;
		case Bytecode::B_LDARG_S:
			instruction->Opcode = Bytecode::B_STARG_S;
			break;
		case Bytecode::B_LOAD:
			instruction->Opcode = Bytecode::B_STORE;
			break;
		case Bytecode::B_LDLOC:
			instruction->Opcode = Bytecode::B_STLOC;
			break;
		case Bytecode::B_LDARG:
			instruction->Opcode = Bytecode::B_STARG;
			break;
		case Bytecode::B_LOAD_L:
			instruction->Opcode = Bytecode::B_STORE_L;
			break;
		case Bytecode::B_LDLOC_L:
			instruction->Opcode = Bytecode::B_STLOC_L;
			break;
		case Bytecode::B_LDARG_L:
			instruction->Opcode = Bytecode::B_STARG_L;
			break;
		}
	}
		break;
	case OperatorType::EQUAL_TO:
		AddInstruction("", Bytecode::B_CEQ);
		break;
	case OperatorType::NOT_EQUAL_TO:
		AddInstruction("", Bytecode::B_CNEQ);
		break;
	case OperatorType::GREATER_THAN:
		AddInstruction("", Bytecode::B_CGT);
		break;
	case OperatorType::LESS_THAN:
		AddInstruction("", Bytecode::B_CLT);
		break;
	case OperatorType::GREATER_THAN_EQUAL_TO:
		AddInstruction("", Bytecode::B_CGTEQ);
		break;
	case OperatorType::LESS_THAN_EQUAL_TO:
		AddInstruction("", Bytecode::B_CLTEQ);
		break;
	}
}

void Compiler::AddInstruction(Ty_string_t label, Bytecode opcode, std::vector<Ty_uint8_t> bytes)
{
	if (m_Instructions.size() > 0)
	{
		if (m_Instructions[m_Instructions.size() - 1].Opcode == Bytecode::B_NOP && m_Instructions[m_Instructions.size() - 1].Label != "" && label == "")
		{
			m_Instructions[m_Instructions.size() - 1].Opcode = opcode;
			m_Instructions[m_Instructions.size() - 1].Bytes = bytes;
			return;
		}
	}
	m_Instructions.push_back({ label, opcode, bytes });
}

void Compiler::BuildMachineCode()
{
	std::vector<Ty_uint8_t> outputBytes;
	for (int i = 0; i < m_Instructions.size(); i++)
	{
		outputBytes.push_back(m_Instructions[i].Opcode);
		for (int j = 0; j < m_Instructions[i].Bytes.size(); j++)
			outputBytes.push_back(m_Instructions[i].Bytes[j]);
	}

	std::ofstream outputFile(m_OutputPath.c_str(), std::ios::out | std::ios::binary);
	outputFile.write((const char*)& outputBytes[0], outputBytes.size());
	outputFile.close();
}
