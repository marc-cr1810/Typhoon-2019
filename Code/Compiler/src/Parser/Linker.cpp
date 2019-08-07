#include "Linker.h"

Linker::Linker()
{}

void Linker::AddFunction(Ty_string_t name, Ty_string_t labelName, int argCount)
{
	m_Functions.push_back({ name, labelName, argCount });
}
