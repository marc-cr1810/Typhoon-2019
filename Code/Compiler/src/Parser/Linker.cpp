#include "Linker.h"

#include <iostream>

Linker::Linker()
{
	m_Functions.push_back({ "syscall", "F_SYSCALL", std::vector<Ty_string_t>(), AccessType::GLOBAL, 0, true });
}

void Linker::Link(Instruction machineLang)
{

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
		if (m_Functions[i].Name == name && m_Functions[i].Args.size() == argCount)
		{
			if (m_Functions[i].Access == AccessType::LOCAL)
				return &m_Functions[i];
			else
				function = &m_Functions[i];
		}
	}

	if (function == nullptr)
		std::cout << "Error: Undefined function \"" << name << "\"" << std::endl;
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
