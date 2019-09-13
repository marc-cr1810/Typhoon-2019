#include "Lexer.h"

Lexer::Lexer()
{ }

void Lexer::TokenizeFile(File file)
{
	std::string s(file.GetBytes().data(), file.GetBytes().size());
	TokenizeString(s);
}

void Lexer::TokenizeString(Ty_string_t string)
{
	Ty_string_t tokenValue = "";

	for (Ty_uint32_t i = 0; i < string.length(); i++)
	{
		char byte = string[i];

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
				for (Ty_string_t rule : m_OperatorExclusionRules)
				{
					
					if (!(tokenValue[0] == '\"' && tokenValue[tokenValue.size() - 1] == '\"') && !(tokenValue[0] == '\'' && tokenValue[tokenValue.size() - 1] == '\''))
					{
						if (std::regex_match(tokenValue + byte, std::regex(rule)))
							goto SkipSeparator;
					}
					else
					{
						if (tokenValue.size() <= 1)
							goto SkipSeparator;
					}
				}
				AddToken(tokenValue);
				tokenValue = byte;
				while (i < string.length())
				{
					if (i + 1 == string.length())
						goto SkipOperator;
					std::string s = tokenValue;
					s += string[i + 1];
					for (OperatorToken _op : OperatorTokenTypes)
					{
						if (std::regex_match(s, std::regex(_op.Value)))
						{
							tokenValue += string[++i];
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
				CheckNextByte:;
				}
			}
		}

	SkipSeparator:
		if (byte != '\n' && byte != '\r')
			tokenValue += byte;

	SkipOperator:
		if (i == string.length() - 1 || byte == m_EndLineChar)
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

	NextByte:;
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
				if (type.Type == TokenType::END && m_Tokens[m_Tokens.size() - 1].Type == TokenType::START)
				{
					m_Tokens.pop_back();
					return;
				}
				m_Tokens.push_back({ type.Type, value });
				if (type.Type == TokenType::NUMBER)
				{
					if (std::regex_match(value, std::regex("^-?0x[\\da-fA-F]*$")))
						m_Tokens[m_Tokens.size() - 1].Value = std::to_string(std::stoi(value, nullptr, 16));
				}
				return;
			}
		}
		m_Tokens.push_back({ TokenType::UNKNOWN_TOKEN, value });
	}
}