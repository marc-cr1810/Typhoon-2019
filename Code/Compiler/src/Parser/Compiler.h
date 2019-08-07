#ifndef Ty_COMPILER_H
#define Ty_COMPILER_H

#include "../Port.h"
#include "AST.h"
#include "Instruction.h"
#include "Linker.h"

class Compiler
{
public:
	Compiler();

	void Compile(AST ast);

	void AddInstruction(Instruction instruction);

	std::vector<Instruction> GetInstructions() const { return m_Instructions; }
private:
	Linker m_Linker;
	std::vector<Instruction> m_Instructions;
};

#endif