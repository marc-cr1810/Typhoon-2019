#ifndef Ty_LEXER_H
#define Ty_LEXER_H

#include "../Port.h"
#include "../Utils/File.h"
#include "Tokens.h"

class Lexer 
{
public:
    Lexer();

    void TokenizeFile(File file);
	void TokenizeString(Ty_string_t string);

    std::vector<Token> GetTokens() const { return m_Tokens; }
private:
    void AddToken(Ty_string_t value);

    std::vector<Token> m_Tokens;

	// Character indicating the end of a line
	char m_EndLineChar = '\n';

    // Separator chars for getting tokens
    char m_SeparatorChars[1] = { ' ' };

	// Rules to check the current token value against to see if it is a valid separator
	Ty_string_t m_SeparatorExclusionRules[2] = { 
		"^(?:(\".*[^\"])|(\'.*[^\']))$", 		// Checks if the token value is the beginning of a string
		"^#.*$"									// Check if the token is a comment
	};

	Ty_string_t m_Comment = "#.*";		// Regex for a comment

	// Rules to check the current token value against to see if it is a valid operator (checks the next character after the operator)
	Ty_string_t m_OperatorExclusionRules[1] = {
		"^-\\d$", 			// Checks if the token value is a negative number
	};
};

#endif