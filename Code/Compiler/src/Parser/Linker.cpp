#include "Linker.h"

#include <iostream>

Linker::Linker()
{
	m_Functions.push_back({ "syscall", "F_SYSCALL", std::vector<Ty_string_t>(), AccessType::GLOBAL, 0, true, true });
}

void Linker::Link(std::vector<Instruction>* machineLang)
{
	int position = 0;

	// Find all branch and function locations
	for (int i = 0; i < machineLang->size(); i++)
	{
		Instruction instruction = machineLang->at(i);
		if (instruction.Label != "")
		{
			if (instruction.Label.substr(0, 2) == "B_")
			{
				Branch* branch = GetBranchFromName(instruction.Label);
				if (position >= (pow(2, 8)))
				{
					for (int j = 0; j < branch->Jumps.size(); j++)
						position++;
				}
				if (position >= (pow(2, 16)))
				{
					for (int j = 0; j < branch->Jumps.size(); j++)
						position += 2;
				}
				branch->Location = position;
				branch->LocationFound = true;
				std::cout << "Branch " << instruction.Label << ", Pos: " << position << std::endl;
			}
			else if (instruction.Label.substr(0, 2) == "F_")
			{
				Function* func = GetFunctionFromLabel(instruction.Label);
				func->Location = position;
				func->LocationFound = true;
				std::cout << "Function " << instruction.Label << ", Pos: " << position << std::endl;
			}
		}
		if (instruction.Opcode == Bytecode::B_BR || instruction.Opcode == Bytecode::B_BRTRUE || instruction.Opcode == Bytecode::B_BRFALSE)
		{
			Ty_string_t name = VectorToString(instruction.Bytes);
			Branch* branch = GetBranchFromName(name);
			if (branch->LocationFound)
				position += IntToBytes(branch->Location).size();
			else
				position++;
			branch->Jumps.push_back(i);
		}
		else if (instruction.Opcode == Bytecode::B_CALL)
		{
			if (VectorToString(instruction.Bytes) == "F_SYSCALL")
				position += 2;
			else
				position += 4;
		}
		else
		{
			position += instruction.Bytes.size();
		}
		position++;
	}

	for (int i = 0; i < machineLang->size(); i++)
	{
		Instruction instruction = machineLang->at(i);
		if (instruction.Opcode == Bytecode::B_BR || instruction.Opcode == Bytecode::B_BRTRUE || instruction.Opcode == Bytecode::B_BRFALSE)
		{
			Ty_string_t name = VectorToString(instruction.Bytes);
			Branch* branch = GetBranchFromName(name);
			if (branch->LocationFound)
			{
				std::vector<Ty_uint8_t> bytes = IntToBytes(branch->Location);
				Bytecode bytecode =
					bytes.size() == 1 ? (
						instruction.Opcode == B_BR ? Bytecode::B_BR_S :
						instruction.Opcode == B_BRTRUE ? Bytecode::B_BRTRUE_S :
						Bytecode::B_BRFALSE_S
						)
					: bytes.size() == 4 ? (
						instruction.Opcode == B_BR ? Bytecode::B_BR_L :
						instruction.Opcode == B_BRTRUE ? Bytecode::B_BRTRUE_L :
						Bytecode::B_BRFALSE_L
						)
					: instruction.Opcode;
				machineLang->at(i).Opcode = bytecode;
				machineLang->at(i).Bytes = bytes;
			}
		}
		else if (instruction.Opcode == Bytecode::B_CALL)
		{
			Function* func = GetFunctionFromLabel(VectorToString(instruction.Bytes));
			if (func == nullptr)
			{
				std::cout << "Unknown function \"" + VectorToString(instruction.Bytes) + "\"" << std::endl;
				return;
			}
			machineLang->at(i).Bytes = {
				(Ty_uint8_t)((func->Location >> 24) & 0xFF),
				(Ty_uint8_t)((func->Location >> 16) & 0xFF),
				(Ty_uint8_t)((func->Location >> 8) & 0xFF),
				(Ty_uint8_t)(func->Location & 0xFF)
			};
		}
	}
}

Function* Linker::AddFunction(Ty_string_t name, Ty_string_t labelName, std::vector<Ty_string_t> args, AccessType access, int scope)
{
	m_Functions.push_back({ name, labelName, args, access, scope });
	return &m_Functions[m_Functions.size() - 1];
}

void Linker::RemoveFunction(Ty_string_t labelName)
{
	for (int i = 0; i < m_Functions.size(); i++)
	{
		if (m_Functions[i].LabelName == labelName && m_Functions[i].Access != AccessType::GLOBAL)
		{
			m_Functions.erase(m_Functions.begin() + i);
			return;
		}
	}
}

void Linker::RemoveFunctions(int scope)
{
	int i = 0;
	while (i < m_Functions.size())
	{
		if (m_Functions[i].Scope == scope && m_Functions[i].Access != AccessType::GLOBAL)
			m_Functions.erase(m_Functions.begin() + i);
		else
			i++;
	}
}

Variable* Linker::AddVariable(Ty_string_t name, Ty_string_t labelName, AccessType access, int scope)
{
	int id = 0;
	Ty_string_t type = labelName.substr(0, 2);
	for (int i = 0; i < m_Variables.size(); i++)
	{
		if (m_Variables[i].LabelName.substr(0, 2) == type)
			id++;
	}
	m_Variables.push_back({ id, name, labelName, access, scope });
	return &m_Variables[m_Variables.size() - 1];
}

void Linker::RemoveVariable(Ty_string_t labelName)
{
	for (int i = 0; i < m_Variables.size(); i++)
	{
		if (m_Variables[i].LabelName == labelName && m_Variables[i].Access != AccessType::GLOBAL)
		{
			m_Variables.erase(m_Variables.begin() + i);
			return;
		}
	}
}

void Linker::RemoveVariables(int scope)
{
	int i = 0;
	while (i < m_Variables.size())
	{
		if (m_Variables[i].Scope == scope && m_Variables[i].Access != AccessType::GLOBAL)
			m_Variables.erase(m_Variables.begin() + i);
		else
			i++;
	}
}

Branch* Linker::AddBranch()
{
	m_Branches.push_back({ "B_" + std::to_string(m_Branches.size()), 0 });
	return &m_Branches[m_Branches.size() - 1];
}

Function* Linker::GetFunctionFromNameArgCount(Ty_string_t name, int argCount)
{
	Function* function = nullptr;

	for (int i = 0; i < m_Functions.size(); i++)
	{
		if (m_Functions[i].Name == name && (m_Functions[i].Args.size() == argCount || m_Functions[i].IgnoreArgCount))
		{
			if (m_Functions[i].Access == AccessType::LOCAL)
				return &m_Functions[i];
			else
				function = &m_Functions[i];
		}
	}

	//if (function == nullptr)
	//	std::cout << "Error: Undefined function \"" << name << "\"" << std::endl;
	return function;
}

Function* Linker::GetFunctionFromLabel(Ty_string_t label)
{
	Function* function = nullptr;

	if (label.size() > 4)
	{
		if (label.substr(0, 4) == "UDF_")
		{
			int i = 4;
			Ty_string_t argCount = "";
			while (label[i] != '_')
				argCount += label[i++];
			Ty_string_t name = "";
			i++;
			while (i < label.size())
				name += label[i++];
			return GetFunctionFromNameArgCount(name, std::stoi(argCount));
		}
	}
	for (int i = 0; i < m_Functions.size(); i++)
	{
		if (m_Functions[i].LabelName == label)
		{
			function = &m_Functions[i];
		}
	}

	if (function == nullptr)
		std::cout << "Error: Unknown function label \"" << label << "\"" << std::endl;
	return function;
}

Variable* Linker::GetVariableFromName(Ty_string_t name)
{
	Variable* variable = nullptr;

	for (int i = 0; i < m_Variables.size(); i++)
	{
		if (m_Variables[i].Name == name)
		{
			if (m_Variables[i].Access == AccessType::LOCAL)
				return &m_Variables[i];
			else
				variable = &m_Variables[i];
		}
	}

	if (variable == nullptr)
		std::cout << "Error: Undefined variable \"" << name << "\"" << std::endl;
	return variable;
}

VariableType Linker::GetVarTypeFromLabel(Ty_string_t label)
{
	Ty_string_t type = label.substr(0, 2);
	return type == "VA" ? VariableType::VAR_ARGUMENT : type == "VL" ? VariableType::VAR_LOCAL : VariableType::VAR_GLOBAL;
}

Branch* Linker::GetBranchFromName(Ty_string_t name)
{
	Branch* branch = nullptr;

	for (int i = 0; i < m_Branches.size(); i++)
	{
		if (m_Branches[i].Name == name)
		{
			branch = &m_Branches[i];
		}
	}

	if (branch == nullptr)
		std::cout << "Error: Unknown branch \"" << name << "\"" << std::endl;
	return branch;
}
