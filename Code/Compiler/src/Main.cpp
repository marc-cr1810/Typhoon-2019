#include "Typhoon.h"

#include "Parser/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/Compiler.h"

Ty_string_t ProgramFilePath = "";
Ty_string_t OutputFilePath = "";
std::vector<Ty_string_t> IncludePaths;

static File GetFile(const Ty_string_t& name)
{
	if (File::Exists(name))
	{
		File file(name);
		return file;
	}
	else
	{
		for (int i = 0; i < IncludePaths.size(); i++)
		{
			Ty_string_t incPath = IncludePaths[i];
			if (incPath.substr(incPath.size() - 1, 1) != "\\" && incPath.substr(incPath.size() - 1, 1) != "/")
				incPath += "/";
			if (File::Exists(incPath + name))
			{
				File file(incPath + name);
				return file;
			}
		}
	}

	std::cout << "File \"" << ProgramFilePath << "\" does not exist!" << std::endl;
	exit(-1);
}

static void PreProcess(File *programFile)
{
	std::vector<char> newBytes;
	std::vector<Ty_string_t> line;
	Ty_string_t token = "";
	for (int i = 0; i < programFile->GetBytes().size(); i++)
	{
		char byte = programFile->GetBytes()[i];
		if (byte == ' ')
		{
			line.push_back(token);
			token = "";
		}
		else if (byte == '\n')
		{
			line.push_back(token);
			token = "";
			if (line[0] == "use")
			{
				if (line[1].size() > 3)
					if (line[1].substr(line[1].size() - 3, 3) != ".ty")
						line[1] += ".ty";
				File useFile = GetFile(line[1]);
				PreProcess(&useFile);
				for (int j = 0; j < useFile.GetBytes().size(); j++)
					newBytes.push_back(useFile.GetBytes()[j]);
				newBytes.push_back('\n');

				std::cout << "Using file \"" << line[1] << "\"" << std::endl;
			}
			else
			{
				for (int j = 0; j < line.size(); j++)
				{
					for (int k = 0; k < line[j].size(); k++)
						newBytes.push_back(line[j][k]);
					if (j != line.size() - 1)
						newBytes.push_back(' ');
				}
				newBytes.push_back('\n');
			}
			line.clear();
		}
		else if (byte != '\r')
			token += byte;
	}

	if (token != "")
		line.push_back(token);

	for (int j = 0; j < line.size(); j++)
	{
		for (int k = 0; k < line[j].size(); k++)
			newBytes.push_back(line[j][k]);
		if (j != line.size() - 1)
			newBytes.push_back(' ');
	}

	programFile->SetBytes(newBytes);
}

int main(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (Ty_string_t(argv[i]) == "-o")
		{
			if (i + 1 < argc)
				OutputFilePath = Ty_string_t(argv[++i]);
		}
		else if (Ty_string_t(argv[i]) == "-I")
		{
			if (i + 1 < argc)
				IncludePaths.push_back(Ty_string_t(argv[++i]));
		}
		else
			ProgramFilePath = Ty_string_t(argv[i]);
	}

	if (ProgramFilePath == "" || OutputFilePath == "")
		return 0;

	File programFile = GetFile(ProgramFilePath);
	std::cout << "Compiling Program" << std::endl;

	PreProcess(&programFile);

	Lexer lexer;
	lexer.TokenizeFile(programFile);

	Parser parser;
	parser.Parse(&lexer);

	Compiler compiler(OutputFilePath);
	compiler.Compile(&parser);

	std::cout << "Finished Compiling Program" << std::endl;
}