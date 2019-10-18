#ifndef Ty_LINKER_H
#define Ty_LINKER_H

#include "../Port.h"
#include "../Utils/Converter.h"
#include "Instruction.h"

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
	int Location = 0;
	bool LocationFound = false;
	std::vector<int> Jumps;
};

struct Function
{
	Ty_string_t Name;
	Ty_string_t LabelName;
	std::vector<Ty_string_t> Args;
	AccessType Access;
	int Scope;
	bool InBuilt = false;
	bool IgnoreArgCount = false;
	bool LocationFound = false;
	int Location = 0;
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

	void Link(std::vector<Instruction>* machineLang);

	Function* AddFunction(Ty_string_t name, Ty_string_t labelName, std::vector<Ty_string_t> args, AccessType access, int scope = 0);
	void RemoveFunction(Ty_string_t labelName);
	void RemoveFunctions(int scope);

	Variable* AddVariable(Ty_string_t name, Ty_string_t labelName, int scope = 0);
	void RemoveVariable(Ty_string_t labelName);
	void RemoveVariables(int scope);

	Branch* AddBranch();

	Function* GetFunctionFromNameArgCount(Ty_string_t name, int argCount);
	Function* GetFunctionFromLabel(Ty_string_t label);
	Variable* GetVariableFromName(Ty_string_t name, bool ignoreWarn = false);
	VariableType GetVarTypeFromLabel(Ty_string_t label);
	Branch* GetBranchFromName(Ty_string_t label);

	std::vector<Function> GetFunctions() const { return m_Functions; }
	std::vector<Variable> GetVariables() const { return m_Variables; }
private:
	std::vector<Function> m_Functions;
	std::vector<Variable> m_Variables;
	std::vector<Branch> m_Branches;
};

#endif