#include "Lexer.h"

#include <iostream>

Lexer::Lexer()
{ }

void Lexer::TokenizeFile(File file)
{
    Ty_string_t tokenValue = "";
    
    for (Ty_uint32_t i = 0; i < file.GetBytes().size(); i++)
    {
        char byte = file.GetBytes()[i];

		// Check if the byte is a separator
        for (char separator : m_SeparatorChars)
        {
            if (byte == separator)
            {
				for (Ty_string_t rule : m_SeparatorExclusionRules)
				{
					if (std::regex_match(tokenValue, std::regex(rule)))
						goto SkipSeparator;
				}
				AddToken(tokenValue);
				tokenValue = "";
				goto NextByte;
            }
        }

		for (Token op : m_OperatorTokenTypes)
		{
			if (std::regex_match(std::string(1, byte), std::regex(op.Value)))
			{
				AddToken(tokenValue);
				tokenValue = byte;
				while (i < file.GetBytes().size())
				{
					for (Token _op : m_OperatorTokenTypes)
					{
						if (i + 1 == file.GetBytes().size())
							break;
						std::string s = tokenValue;
						s += file.GetBytes()[i + 1];
						if (std::regex_match(s, std::regex(_op.Value)))
						{
							tokenValue += file.GetBytes()[++i];
							goto CheckNextByte;
						}
					}
					AddToken(tokenValue);
					tokenValue = "";
					goto NextByte;
				CheckNextByte: ;
				}
			}
		}
	SkipSeparator:

		if (byte != '\n' && byte != '\r')
			tokenValue += byte;

		if (i == file.GetBytes().size() - 1 || byte == '\n')
		{
			AddToken(tokenValue);
			tokenValue = "";
		}

    NextByte: ;
    }
}

void Lexer::AddToken(Ty_string_t value)
{
	if (value != "")
	{
		for (Token type : m_TokenTypes)
		{
			if (std::regex_match(value, std::regex(type.Value)))
			{
				m_Tokens.push_back({ type.Type, value });
				return;
			}
		}
		for (Token opType : m_OperatorTokenTypes)
		{
			if (std::regex_match(value, std::regex(opType.Value)))
			{
				m_Tokens.push_back({ opType.Type, value });
				return;
			}
		}
		m_Tokens.push_back({ TokenType::UNNOWN, value });
	}
}