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

enum Bytecode
{
	NOP,
	BREAK,
	LDARG_S,
	STARG_S,
	LDARG,
	STARG,
	LDARG_L,
	STARG_L,
	LDLOC_S,
	STLOC_S,
	LDLOC,
	STLOC,
	LDLOC_L,
	STLOC_S,
	LOAD_S,
	STORE_S,
	LOAD,
	STORE,
	LOAD_L,
	STORE_L,
	LDNULL,
	LDCONST_S,
	LDUCONST_S,
	LDCONST,
	LDUCONST,
	LDCONST_L,
	LDUCONST_L,
	LDFLOAT_S,
	LDFLOAT_D,
	LDSTR,
	LDTRUE,
	LDFALSE,
	BR_S,
	BRTRUE_S,
	BRFALSE_S,
	BR,
	BRTRUE,
	BRFALSE,
	BR_L,
	BRTRUE_L,
	BRFALSE_L,
	DUP,
	POP,
	CALL,
	RET,
	ADD,
	SUB,
	MUL,
	DIV,
	REM,
	CEQ,
	CNEQ,
	CGT,
	CLT,
	SYSCALL
};

#endif