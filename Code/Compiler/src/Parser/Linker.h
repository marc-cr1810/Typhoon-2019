#ifndef Ty_LINKER_H
#define Ty_LINKER_H

#include "../Port.h"

enum AccessType
{
	GLOBAL,
	LOCAL
};

struct Function
{
	Ty_string_t Name;
	Ty_string_t LabelName;
	std::vector<Ty_string_t> Args;
	AccessType Access;
	int Level;
	bool InBuilt = false;
};

struct Variable
{
	int ID;
	Ty_string_t Name;
	Ty_string_t LabelName;
	AccessType Access;
	int Level;
};

class Linker
{
public:
	Linker();

	void AddFunction(Ty_string_t name, Ty_string_t labelName, std::vector<Ty_string_t> args, AccessType access, int level = 0);
	void RemoveFunction(Ty_string_t labelName);
	void RemoveFunctions(int level);

	void AddVariable(Ty_string_t name, Ty_string_t labelName, AccessType access, int level = 0);
	void RemoveVariable(Ty_string_t labelName);
	void RemoveVariables(int level);

	std::vector<Function> GetFunctions() const { return m_Functions; }
	std::vector<Variable> GetVariables() const { return m_Variables; }
private:
	std::vector<Function> m_Functions;
	std::vector<Variable> m_Variables;
};

#endif