#ifndef Ty_COMPILER_H
#define Ty_COMPILER_H

#include "../Port.h"
#include "Linker.h"
#include "Parser.h"
#include "AST.h"

enum Bytecode
{
	B_NOP,
	B_BREAK,
	B_LDARG_S,
	B_STARG_S,
	B_LDARG,
	B_STARG,
	B_LDARG_L,
	B_STARG_L,
	B_LDLOC_S,
	B_STLOC_S,
	B_LDLOC,
	B_STLOC,
	B_LDLOC_L,
	B_STLOC_L,
	B_LOAD_S,
	B_STORE_S,
	B_LOAD,
	B_STORE,
	B_LOAD_L,
	B_STORE_L,
	B_LDNULL,
	B_LDCONST_S,
	B_LDUCONST_S,
	B_LDCONST,
	B_LDUCONST,
	B_LDCONST_L,
	B_LDUCONST_L,
	B_LDFLOAT_S,
	B_LDFLOAT_D,
	B_LDSTR,
	B_LDTRUE,
	B_LDFALSE,
	B_BR_S,
	B_BRTRUE_S,
	B_BRFALSE_S,
	B_BR,
	B_BRTRUE,
	B_BRFALSE,
	B_BR_L,
	B_BRTRUE_L,
	B_BRFALSE_L,
	B_DUP,
	B_POP,
	B_CALL,
	B_RET,
	B_ADD,
	B_SUB,
	B_MUL,
	B_DIV,
	B_REM,
	B_CEQ,
	B_CNEQ,
	B_CGT,
	B_CLT,
	B_SYSCALL
};

struct Instruction
{
	Ty_string_t Label;
	Bytecode Opcode;
	std::vector<Ty_uint8_t> Bytes;
};

class Compiler
{
public:
	Compiler();

	void Compile(Parser *parser);
	void CompileASTNode(Node ast, int level = 0);

	void AddInstruction(Ty_string_t label, Bytecode opcode, std::vector<Ty_uint8_t> bytes = std::vector<Ty_uint8_t>());

	std::vector<Instruction> GetInstructions() const { return m_Instructions; }
private:
	Linker m_Linker;
	std::vector<Instruction> m_Instructions;
};

#endif