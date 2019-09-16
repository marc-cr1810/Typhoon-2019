#ifndef Ty_FILE_H
#define Ty_FILE_H

#include "../Port.h"

class File {
public:
	File()
	{ }

    File(Ty_string_t filename)
        : m_Filename(filename)
    {
        m_Bytes = ReadAllBytes(filename.c_str());
    }

	Ty_string_t GetFilename() const { return m_Filename; }
    std::vector<char> GetBytes() const { return m_Bytes; }
	void SetBytes(std::vector<char> bytes) { m_Bytes = bytes; }

    static std::vector<char> ReadAllBytes(char const* filename)
    {
        std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        std::vector<char> result(pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&result[0], pos);

        return result;
    }

	static bool Exists(const Ty_string_t& name)
	{
		struct stat buffer;
		return (stat(name.c_str(), &buffer) == 0);
	}
private:
	Ty_string_t m_Filename;
    std::vector<char> m_Bytes;
};

#endif