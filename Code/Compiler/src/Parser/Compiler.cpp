#include "Compiler.h"

Compiler::Compiler()
{}

void Compiler::Compile(Parser* parser)
{
	CompileASTNode(parser->GetAST().Program);
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

				int startPoint = m_Instructions.size();
				CompileASTBlock(node.Children[args.size() > 0 ? 1 : 0], scope + 1);
				AddInstruction("", Bytecode::B_RET);
				m_Instructions[startPoint].Label = label;
				
				std::cout << label << ": " << name << std::endl;
			}
			else if (node.StmtType == StatementType::IF)
			{
				Branch* endPoint = m_Linker.AddBranch();
				CompileASTNode(node.Children[0], scope);
				AddInstruction("", Bytecode::B_BRFALSE, StringToVector(endPoint->Name));
				CompileASTBlock(node.Children[1], scope + 1);

				if (node.Children.size() > 2)
				{
					Node* n = &node.Children[2];
					while (n->Children.size() > 2)
					{
						AddInstruction("", Bytecode::B_BR, StringToVector(endPoint->Name));
						CompileASTNode(n->Children[0], scope);
						AddInstruction("", Bytecode::B_BRFALSE, StringToVector(endPoint->Name));
						CompileASTBlock(n->Children[1], scope + 1);
						if (n->Children.size() > 2)
							n = &n->Children[2];
						else
							break;
					}
					if (n->StmtType == StatementType::ELSE)
					{
						CompileASTBlock(n->Children[0], scope + 1);
					}
				}

				AddInstruction(endPoint->Name, Bytecode::B_NOP);
				std::cout << node.Value << std::endl;
			}
			else if (node.StmtType == StatementType::ASSIGN_NEW)
			{
				Ty_string_t label = (scope == 0 ? "VG_" : "VL_") + std::to_string(m_Linker.GetVariables().size());
				Ty_string_t name = node.Children[0].Value;
				Variable* var = m_Linker.AddVariable(name, label, scope == 0 ? AccessType::GLOBAL : AccessType::LOCAL, scope);
				
				if (node.Children.size() > 1)
					CompileASTNode(node.Children[1], scope);
				else
					AddInstruction("", Bytecode::B_LDNULL);

				Bytecode bytecode;
				if (var->ID < (pow(2, 8) - 1))
					bytecode = scope == 0 ? Bytecode::B_STORE_S : Bytecode::B_STLOC_S;
				else if (var->ID < (pow(2, 16) - 1))
					bytecode = scope == 0 ? Bytecode::B_STORE : Bytecode::B_STLOC;
				else if (var->ID < (pow(2, 32) - 1))
					bytecode = scope == 0 ? Bytecode::B_STORE_L : Bytecode::B_STLOC_L;
				AddInstruction("", bytecode, StringToVector(label));
			}
			else if (node.StmtType == StatementType::EXPRESSION)
			{
				CompileASTNode(node.Children[0], scope);
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
			Variable* var = m_Linker.GetVariableFromName(object.Value);
			VariableType type = m_Linker.GetVarTypeFromLabel(var->LabelName);
			std::vector<Ty_uint8_t> bytes = IntToBytes(var->ID);
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
			break;
		case ObjectType::OBJ_STRING:
		{
			Ty_string_t value = object.Value.substr(1, object.Value.length() - 2);
			AddInstruction("", Bytecode::B_LDSTR, StringToVector(value));
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
			AddInstruction("", Bytecode::B_CALL, StringToVector(function->LabelName));
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
	m_Instructions.push_back({ label, opcode, bytes });
}
