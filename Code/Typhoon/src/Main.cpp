#include "Port.h"
#include "VirtualMachine.h"

int main(int argc, char** argv)
{
	Ty_string_t runFile;
	for (int i = 1; i < argc; ++i)
	{
		if (Ty_string_t(argv[i]) == "-tyc")
		{
			if (i + 1 < argc)
			{
				runFile = Ty_string_t(argv[++i]);
			}
		}
		else
			runFile = Ty_string_t(argv[i]);

		File programFile(runFile.c_str());
		VirtualMachine vm;
		vm.Run(programFile);
	}
}