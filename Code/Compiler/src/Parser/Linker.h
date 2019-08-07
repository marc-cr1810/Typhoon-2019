#ifndef Ty_LINKER_H
#define Ty_LINKER_H

#include "../Port.h"

struct Function
{
	Ty_string_t Name;
	Ty_string_t LabelName;
	int ArgCount = 0;
};

class Linker
{
public:
	Linker();

	void AddFunction(Ty_string_t name, Ty_string_t labelName, int argCount);

	std::vector<Function> GetFunctions() const { return m_Functions; }
private:
	std::vector<Function> m_Functions;
};

#endif