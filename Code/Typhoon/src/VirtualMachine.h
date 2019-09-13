#ifndef Ty_VIRTUALMACHINE_H
#define Ty_VIRTUALMACHINE_H

#include "Typhoon.h"

class VirtualMachine
{
public:
	VirtualMachine();

	void Run(File file);
private:
	Ty_uint64_t m_PC = 0;
	std::vector<char> m_Bytecode;

	std::stack<TyObject> m_Stack;
	std::stack<Ty_uint64_t> m_ReturnPositions;

	std::vector<TyObject> m_GlobalVars;
	std::vector<std::vector<TyObject>> m_LocalVars;
	std::vector<std::vector<TyObject>> m_ArgVars;

	void Syscall(SyscallCode code);

	TyObject* GetLocalVar(int index)
	{
		if (m_LocalVars.size() > 0)
			return &m_LocalVars[m_LocalVars.size() - 1][index];
		else
			return &Ty_NULL;
	}

	TyObject* GetArgVar(int index)
	{
		if (m_ArgVars.size() > 0)
			return &m_ArgVars[m_ArgVars.size() - 1][index];
		else
			return &Ty_NULL;
	}

	void LoadGlobalVarToStack(int index)
	{
		if (index < m_GlobalVars.size())
			m_Stack.push(m_GlobalVars[index]);
		else
			std::cout << "Inaccessible memory location, missing global variable!" << std::endl;
	}

	void StoreGlobalVarFromStack(int index)
	{
		if (index < m_GlobalVars.size())
			m_GlobalVars[index] = m_Stack.top();
		else
			m_GlobalVars.push_back(m_Stack.top());
		m_Stack.pop();
	}

	void LoadLocalVarToStack(int index)
	{
		if (index < m_LocalVars[m_LocalVars.size() - 1].size())
			m_Stack.push(*GetLocalVar(index));
		else
			std::cout << "Inaccessible memory location, missing local variable!" << std::endl;
	}

	void StoreLocalVarFromStack(int index)
	{
		if (index < m_LocalVars[m_LocalVars.size() - 1].size())
			m_LocalVars[m_LocalVars.size() - 1][index] = m_Stack.top();
		else
			m_LocalVars[m_LocalVars.size() - 1].push_back(m_Stack.top());
		m_Stack.pop();
	}

	void LoadArgVarToStack(int index)
	{
		if (index < m_ArgVars[m_ArgVars.size() - 1].size())
			m_Stack.push(*GetArgVar(index));
		else
			std::cout << "Inaccessible memory location, missing argument variable!" << std::endl;
	}

	void StoreArgVarFromStack(int index)
	{
		if (index < m_ArgVars[m_ArgVars.size() - 1].size())
			m_ArgVars[m_ArgVars.size() - 1][index] = m_Stack.top();
		else
			m_ArgVars[m_ArgVars.size() - 1].push_back(m_Stack.top());
		m_Stack.pop();
	}
};

#endif