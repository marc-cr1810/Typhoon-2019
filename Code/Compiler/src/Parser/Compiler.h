#ifndef Ty_COMPILER_H
#define Ty_COMPILER_H

#include "../Port.h"
#include "../Utils/Converter.h"
#include "Instruction.h"
#include "Linker.h"
#include "Parser.h"
#include "AST.h"

class Compiler
{
public:
	Compiler();

	void Compile(Parser *parser);
	void CompileASTBlock(Node block, int scope = 0);
	void CompileASTNode(Node ast, int scope = 0);
	void CompileObject(Node object, int scope = 0);
	void CompileExpression(Node object, int scope = 0);

	void AddInstruction(Ty_string_t label, Bytecode opcode, std::vector<Ty_uint8_t> bytes = std::vector<Ty_uint8_t>());

	std::vector<Instruction> GetInstructions() const { return m_Instructions; }
private:
	Linker m_Linker;
	std::vector<Instruction> m_Instructions;
};

#endif