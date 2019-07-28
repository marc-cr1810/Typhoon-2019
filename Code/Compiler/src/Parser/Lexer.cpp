#include "Lexer.h"

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

		for (OperatorToken op : OperatorTokenTypes)
		{
			if (std::regex_match(std::string(1, byte), std::regex(op.Value)))
			{
				AddToken(tokenValue);
				tokenValue = byte;
				while (i < file.GetBytes().size())
				{
					if (i + 1 == file.GetBytes().size())
						break;
					std::string s = tokenValue;
					s += file.GetBytes()[i + 1];
					for (OperatorToken _op : OperatorTokenTypes)
					{
						if (std::regex_match(s, std::regex(_op.Value)))
						{
							tokenValue += file.GetBytes()[++i];
							goto CheckNextByte;
						}
					}

					for (Ty_string_t rule : m_OperatorExclusionRules)
					{
						if (std::regex_match(s, std::regex(rule)))
							goto SkipOperator;
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

	SkipOperator:
		if (i == file.GetBytes().size() - 1 || byte == m_EndLineChar)
		{
			if (!std::regex_match(tokenValue, std::regex(m_Comment)))
				AddToken(tokenValue);
			
			if (m_Tokens.size() > 0)
			{
				if (m_Tokens[m_Tokens.size() - 1].Type != TokenType::END)
					AddToken(std::string(1, m_EndLineChar));
			}
			tokenValue = "";
		}

    NextByte: ;
    }
}

void Lexer::AddToken(Ty_string_t value)
{
	if (value != "")
	{
		for (OperatorToken opType : OperatorTokenTypes)
		{
			if (std::regex_match(value, std::regex(opType.Value)))
			{
				m_Tokens.push_back({ opType.Type, value });
				return;
			}
		}
		for (Token type : TokenTypes)
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