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
};

#endif