#ifndef Ty_FILE_H
#define Ty_FILE_H

#include "../Port.h"

#include <fstream>

class File {
public:
    File(const char* filename)
        : m_Filename(filename)
    {
        m_Bytes = ReadAllBytes(filename);
    }

    const char* GetFilename() const { return m_Filename; }
    std::vector<char> GetBytes() const { return m_Bytes; }

    static std::vector<char> ReadAllBytes(char const* filename)
    {
        std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type pos = ifs.tellg();

        std::vector<char> result(pos);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&result[0], pos);

        return result;
    }
private:
    const char* m_Filename;
    std::vector<char> m_Bytes;
};

#endif