#include "Linker.h"

Linker::Linker()
{
	m_Functions.push_back({ "syscall", "F_SYSCALL", std::vector<Ty_string_t>(), AccessType::GLOBAL, 0, true });
}

void Linker::AddFunction(Ty_string_t name, Ty_string_t labelName, std::vector<Ty_string_t> args, AccessType access, int level)
{
	m_Functions.push_back({ name, labelName, args, access, level });
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

void Linker::RemoveFunctions(int level)
{
	int i = 0;
	while (i < m_Functions.size())
	{
		if (m_Functions[i].Level == level && m_Functions[i].Access != AccessType::GLOBAL)
			m_Functions.erase(m_Functions.begin() + i);
		else
			i++;
	}
}

void Linker::AddVariable(Ty_string_t name, Ty_string_t labelName, AccessType access, int level)
{
	int id = 0;
	Ty_string_t type = labelName.substr(0, 2);
	for (int i = 0; i < m_Variables.size(); i++)
	{
		if (m_Variables[i].LabelName.substr(0, 2) == type)
			id++;
	}
	m_Variables.push_back({ id, name, labelName, access, level });
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

void Linker::RemoveVariables(int level)
{
	int i = 0;
	while (i < m_Variables.size())
	{
		if (m_Variables[i].Level == level && m_Variables[i].Access != AccessType::GLOBAL)
			m_Variables.erase(m_Variables.begin() + i);
		else
			i++;
	}
}
