#include "Lexer.h"

Lexer::Lexer()
{ }

void Lexer::TokenizeFile(File file)
{
    Ty_string_t tokenValue = "";
    
    for (Ty_uint32_t i = 0; i < file.GetBytes().size(); i++)
    {
        char byte = file.GetBytes()[i];
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
		m_Tokens.push_back({ TokenType::UNNOWN, value });
	}
}