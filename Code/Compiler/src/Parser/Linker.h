#ifndef Ty_LINKER_H
#define Ty_LINKER_H

#include "../Port.h"

enum AccessType
{
	GLOBAL,
	LOCAL
};

enum VariableType
{
	VAR_GLOBAL,
	VAR_LOCAL,
	VAR_ARGUMENT
};

struct Branch
{
	Ty_string_t Name;
	int Location;
};

struct Function
{
	Ty_string_t Name;
	Ty_string_t LabelName;
	std::vector<Ty_string_t> Args;
	AccessType Access;
	int Scope;
	bool InBuilt = false;
};

struct Variable
{
	int ID;
	Ty_string_t Name;
	Ty_string_t LabelName;
	AccessType Access;
	int Scope;
};

class Linker
{
public:
	Linker();

	Function* AddFunction(Ty_string_t name, Ty_string_t labelName, std::vector<Ty_string_t> args, AccessType access, int scope = 0);
	void RemoveFunction(Ty_string_t labelName);
	void RemoveFunctions(int scope);

	Variable* AddVariable(Ty_string_t name, Ty_string_t labelName, AccessType access, int scope = 0);
	void RemoveVariable(Ty_string_t labelName);
	void RemoveVariables(int scope);

	Branch* AddBranch();

	Function* GetFunctionFromNameArgCount(Ty_string_t name, int argCount);
	Variable* GetVariableFromName(Ty_string_t name);
	VariableType GetVarTypeFromLabel(Ty_string_t label);

	std::vector<Function> GetFunctions() const { return m_Functions; }
	std::vector<Variable> GetVariables() const { return m_Variables; }
private:
	std::vector<Function> m_Functions;
	std::vector<Variable> m_Variables;
	std::vector<Branch> m_Branches;
};

#endif