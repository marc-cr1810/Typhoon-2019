#ifndef Ty_VIRTUALMACHINE_H
#define Ty_VIRTUALMACHINE_H

#include "Typhoon.h"

class VirtualMachine
{
public:
	VirtualMachine();

	void Run(File file);
private:
	Ty_uint64_t m_PC;
	std::vector<char> m_Bytecode;
};

#endif