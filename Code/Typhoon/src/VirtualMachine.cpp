#include "VirtualMachine.h"

VirtualMachine::VirtualMachine()
{}

void VirtualMachine::Run(File file)
{
	m_Bytecode = file.GetBytes();

	while (m_PC < m_Bytecode.size())
	{
		Opcode opcode = (Opcode)m_Bytecode[m_PC];
		if (opcode != Opcode::B_NOP)
		{
			switch (opcode)
			{
			case B_BREAK:
				m_PC++;
				break;
			case B_LDARG_S:
				break;
			case B_STARG_S:
				break;
			case B_LDARG:
				break;
			case B_STARG:
				break;
			case B_LDARG_L:
				break;
			case B_STARG_L:
				break;
			case B_LDLOC_S:
				break;
			case B_STLOC_S:
				break;
			case B_LDLOC:
				break;
			case B_STLOC:
				break;
			case B_LDLOC_L:
				break;
			case B_STLOC_L:
				break;
			case B_LOAD_S:
				break;
			case B_STORE_S:
				break;
			case B_LOAD:
				break;
			case B_STORE:
				break;
			case B_LOAD_L:
				break;
			case B_STORE_L:
				break;
			case B_LDNULL:
				break;
			case B_LDCONST_S:
				break;
			case B_LDUCONST_S:
				break;
			case B_LDCONST:
				break;
			case B_LDUCONST:
				break;
			case B_LDCONST_L:
				break;
			case B_LDUCONST_L:
				break;
			case B_LDFLOAT_S:
				break;
			case B_LDFLOAT_D:
				break;
			case B_LDSTR:
				break;
			case B_LDTRUE:
				break;
			case B_LDFALSE:
				break;
			case B_BR_S:
				break;
			case B_BRTRUE_S:
				break;
			case B_BRFALSE_S:
				break;
			case B_BR:
				break;
			case B_BRTRUE:
				break;
			case B_BRFALSE:
				break;
			case B_BR_L:
				break;
			case B_BRTRUE_L:
				break;
			case B_BRFALSE_L:
				break;
			case B_DUP:
				break;
			case B_POP:
				break;
			case B_CALL:
				break;
			case B_RET:
				break;
			case B_ADD:
				break;
			case B_SUB:
				break;
			case B_MUL:
				break;
			case B_DIV:
				break;
			case B_REM:
				break;
			case B_CEQ:
				break;
			case B_CNEQ:
				break;
			case B_CGT:
				break;
			case B_CLT:
				break;
			case B_CGTEQ:
				break;
			case B_CLTEQ:
				break;
			case B_SYSCALL:
				break;
			default:
				m_PC++;
				break;
			}
		}
		else
			m_PC++;
	}
}
