#ifndef Ty_COMPILER_H
#define Ty_COMPILER_H

#include "../Port.h"
#include "AST.h"
#include "Instruction.h"

class Compiler
{
public:
	Compiler();

	void Compile(AST ast);

	std::vector<Instruction> GetInstructions() const { return m_Instructions; }
private:
	std::vector<Instruction> m_Instructions;
};

#endif