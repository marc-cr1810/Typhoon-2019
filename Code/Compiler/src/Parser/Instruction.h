#ifndef Ty_INSTRUCTION_H
#define Ty_INSTRUCTION_H

/*	!!!!!! MAINTAINING BACKWARDS COMPATIBILITY IS EXTREMELY IMPORTANT ACCROSS ALL COMPILED EXECUTABLES !!!!!!
//	To ensure this all new instructions added must come AFTER the previously created ones.
//	This will make sure that there is no chance that one instruction compiled with an older version
//	of this compiler will be different in future versions.
//
//	For example:
//		Currently the instruction "ADD" has an opcode of 45 (0x2D).
//		If an insutruction was to be inserted before "ADD" this would change it's opcode to be 46 (0x2E)
//
//		The instruction "RET" would then be what "ADD" used to be and in any older programs instead of using
//		the instruction "ADD" it would be using "RET" instead, breaking the program.
//
//	This is not ideal as backwards compatibility with programs compiled in older versions of the compiler should
//	still continue to function as expected.
*/

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
	B_CGTEQ,
	B_CLTEQ,
	B_SYSCALL,
	B_NEWARRAY,
	B_ALOAD,
	B_ASTORE
};

struct Instruction
{
	Ty_string_t Label;
	Bytecode Opcode;
	std::vector<Ty_uint8_t> Bytes;
};

#endif