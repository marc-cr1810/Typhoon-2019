#include "VirtualMachine.h"

VirtualMachine::VirtualMachine()
{
	m_LocalVars.push_back(std::vector<TyObject>());
	m_ArgVars.push_back(std::vector<TyObject>());
}

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
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				LoadArgVarToStack(index);
				m_PC += 2;
			}
				break;
			case B_STARG_S:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				StoreArgVarFromStack(index);
				m_PC += 2;
			}
				break;
			case B_LDARG:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				LoadArgVarToStack(index);
				m_PC += 3;
			}
				break;
			case B_STARG:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				StoreArgVarFromStack(index);
				m_PC += 3;
			}
				break;
			case B_LDARG_L:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				LoadArgVarToStack(index);
				m_PC += 5;
			}
				break;
			case B_STARG_L:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				StoreArgVarFromStack(index);
				m_PC += 5;
			}
				break;
			case B_LDLOC_S:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				LoadLocalVarToStack(index);
				m_PC += 2;
			}
				break;
			case B_STLOC_S:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				StoreLocalVarFromStack(index);
				m_PC += 2;
			}
				break;
			case B_LDLOC:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				LoadLocalVarToStack(index);
				m_PC += 3;
			}
				break;
			case B_STLOC:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				StoreLocalVarFromStack(index);
				m_PC += 3;
			}
				break;
			case B_LDLOC_L:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				LoadLocalVarToStack(index);
				m_PC += 5;
			}
				break;
			case B_STLOC_L:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				StoreLocalVarFromStack(index);
				m_PC += 5;
			}
				break;
			case B_LOAD_S:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				LoadGlobalVarToStack(index);
				m_PC += 2;
			}
				break;
			case B_STORE_S:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				StoreGlobalVarFromStack(index);
				m_PC += 2;
			}
				break;
			case B_LOAD:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				LoadGlobalVarToStack(index);
				m_PC += 3;
			}
				break;
			case B_STORE:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				StoreGlobalVarFromStack(index);
				m_PC += 3;
			}
				break;
			case B_LOAD_L:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				LoadGlobalVarToStack(index);
				m_PC += 5;
			}
				break;
			case B_STORE_L:
			{
				Ty_uint32_t index = BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				StoreGlobalVarFromStack(index);
				m_PC += 5;
			}
				break;
			case B_LDNULL:
				m_Stack.push({ ObjectType::OBJECT_NULL });
				m_PC++;
				break;
			case B_LDCONST_S:
			{
				TyInt object;
				object.Set(BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] }, true));
				m_Stack.push(object);
				m_PC += 2;
			}
				break;
			case B_LDUCONST_S:
			{
				TyInt object;
				object.Set((Ty_uint32_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] }));
				m_Stack.push(object);
				m_PC += 2;
			}
				break;
			case B_LDCONST:
			{
				TyInt object;
				object.Set(BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] }, true));
				m_Stack.push(object);
				m_PC += 3;
			}
				break;
			case B_LDUCONST:
			{
				TyInt object;
				object.Set((Ty_uint32_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] }));
				m_Stack.push(object);
				m_PC += 3;
			}
				break;
			case B_LDCONST_L:
			{
				TyInt object;
				object.Set(BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] }, true));
				m_Stack.push(object);
				m_PC += 5;
			}
				break;
			case B_LDUCONST_L:
			{
				TyInt object;
				object.Set((Ty_uint32_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] }));
				m_Stack.push(object);
				m_PC += 5;
			}
				break;
			case B_LDFLOAT_S:
			{
				TyInt object;
				object.Set(BytesToFloat({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] }));
				m_Stack.push(object);
				m_PC += 5;
			}
				break;
			case B_LDFLOAT_D:
			{
				/* Make it work for doubles rather than treat them as floats */
				TyInt object;
				object.Set((double)BytesToFloat({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] }));
				m_Stack.push(object);
				m_PC += 5;
			}
				break;
			case B_LDSTR:
			{
				TyString object;
				Ty_string_t value = "";
				m_PC++;
				while (m_Bytecode[m_PC] != '\0')
				{
					value += m_Bytecode[m_PC];
					m_PC++;
				}
				object.Set(value);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_LDTRUE:
			{
				TyBool object;
				object.Set(true);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_LDFALSE:
			{
				TyBool object;
				object.Set(false);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_BR_S:
				m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				break;
			case B_BRTRUE_S:
			{
				if (m_Stack.top().ValueBool)
					m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				else
					m_PC += 2;
				m_Stack.pop();
			}
				break;
			case B_BRFALSE_S:
			{
				if (!m_Stack.top().ValueBool)
					m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1] });
				else
					m_PC += 2;
				m_Stack.pop();
			}
				break;
			case B_BR:
				m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				break;
			case B_BRTRUE:
			{
				if (m_Stack.top().ValueBool)
					m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				else
					m_PC += 3;
				m_Stack.pop();
			}
				break;
			case B_BRFALSE:
			{
				if (!m_Stack.top().ValueBool)
					m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				else
					m_PC += 3;
				m_Stack.pop();
			}
				break;
			case B_BR_L:
				m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				break;
			case B_BRTRUE_L:
			{
				if (m_Stack.top().ValueBool)
					m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				else
					m_PC += 5;
				m_Stack.pop();
			}
				break;
			case B_BRFALSE_L:
			{
				if (!m_Stack.top().ValueBool)
					m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
				else
					m_PC += 5;
				m_Stack.pop();
			}
				break;
			case B_DUP:
			{
				TyObject object = m_Stack.top();
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_POP:
				if (m_Stack.size() > 0)
					m_Stack.pop();
				m_PC++;
				break;
			case B_CALL:
			{
				m_ReturnPositions.push(m_PC + 5);
				m_PC = (Ty_uint64_t)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2], (Ty_uint8_t)m_Bytecode[m_PC + 3], (Ty_uint8_t)m_Bytecode[m_PC + 4] });
			}
				break;
			case B_RET:
			{
				if (m_ReturnPositions.size() > 0)
				{
					m_PC = m_ReturnPositions.top();
					m_ReturnPositions.pop();
				}
				else
					m_PC++;
			}
				break;
			case B_ADD:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				m_Stack.push(left + right);
				m_PC++;
			}
				break;
			case B_SUB:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				m_Stack.push(left - right);
				m_PC++;
			}
				break;
			case B_MUL:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				m_Stack.push(left * right);
				m_PC++;
			}
				break;
			case B_DIV:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				m_Stack.push(left / right);
				m_PC++;
			}
				break;
			case B_REM:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				m_Stack.push(left % right);
				m_PC++;
			}
				break;
			case B_CEQ:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				TyObject object;
				object.Set(left == right);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_CNEQ:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				TyObject object;
				object.Set(left != right);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_CGT:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				TyObject object;
				object.Set(left > right);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_CLT:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				TyObject object;
				object.Set(left < right);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_CGTEQ:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				TyObject object;
				object.Set(left >= right);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_CLTEQ:
			{
				TyObject left = m_Stack.top();
				m_Stack.pop();
				TyObject right = m_Stack.top();
				m_Stack.pop();
				TyObject object;
				object.Set(left <= right);
				m_Stack.push(object);
				m_PC++;
			}
				break;
			case B_SYSCALL:
			{
				SyscallCode code = (SyscallCode)BytesToInt({ (Ty_uint8_t)m_Bytecode[m_PC + 1], (Ty_uint8_t)m_Bytecode[m_PC + 2] });
				Syscall(code);
				m_PC += 3;
			}
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

void VirtualMachine::Syscall(SyscallCode code)
{
	switch (code)
	{
	case SYS_NOP:
		return;
	case SYS_EXIT:
		exit((*GetArgVar(0)).ValueInt);
		break;
	case SYS_KILL:
		abort();
		break;
	case SYS_BREAK:
		break;
	case SYS_FILE_READ:
		break;
	case SYS_FILE_WRITE:
		break;
	case SYS_FILE_DELETE:
		break;
	case SYS_DIR_MAKE:
		break;
	case SYS_DIR_DELETE:
		break;
	case SYS_CLOCK_GETTIME:
		break;
	case SYS_THREAD_SLEEP:
		break;
	case SYS_THREAD_SLEEPMICRO:
		break;
	case SYS_THREAD_SLEEPNANO:
		break;
	case SYS_CONSOLE_READ:
		break;
	case SYS_CONSOLE_READLINE:
		break;
	case SYS_CONSOLE_WRITE:
		std::cout << (*GetArgVar(0)).ValueString;
		break;
	default:
		break;
	}
}
