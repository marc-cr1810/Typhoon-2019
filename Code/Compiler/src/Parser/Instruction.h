#ifndef Ty_INSTRUCTION_H
#define Ty_INSTRUCTION_H

#include "../Port.h"

struct Instruction
{
	Ty_string_t Label;
	Ty_int8_t Opcode;
	std::vector<Ty_uint8_t> Bytes;
};

#endif