#include "Lexer.h"

#include <iostream>

Lexer::Lexer()
{ }

void Lexer::TokenizeFile(File file)
{
    Ty_string_t tokenValue = "";
    
    for (unsigned int i = 0; i < file.GetBytes().size(); i++)
    {
        char byte = file.GetBytes()[i];
        for (char seperator : m_SeperatorChars)
        {
            std::cout << byte << std::endl;
            if (byte == seperator)
            {
                AddToken(tokenValue);
                tokenValue = "";
                goto NextByte;
            }
        }
        tokenValue += byte;

        NextByte: ;
    }
}

void Lexer::AddToken(Ty_string_t value) 
{
    for (Token type : m_TokenTypes)
    {
        if (std::regex_match(value, std::regex(type.Value)))
        {
            m_Tokens.push_back({ type.Type, value });
            return;
        }
    }
    m_Tokens.push_back({ TokenType::UNNOWN, value });
}