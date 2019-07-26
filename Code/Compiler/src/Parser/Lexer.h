#ifndef Ty_LEXER_H
#define Ty_LEXER_H

#include <regex>

#include "../Port.h"
#include "../Utils/File.h"
#include "Token.h"

class Lexer {
public:
    Lexer();

    void TokenizeFile(File file);

    std::vector<Token> GetTokens() const { return m_Tokens; }
private:
    void AddToken(Ty_string_t value);

    std::vector<Token> m_Tokens;

    // Separator chars for getting tokens
    char m_SeparatorChars[1] = { ' ' };

	// Rules to check the current token value against to see if it is a valid seperator
	Ty_string_t m_SeparatorExclusionRules[1] = { "^(?:(\".*)|(\'.*))$" };		// Checks if the token value is the beginning of a string

    // Different token types, uses regex in the value to find the tokens
    Token m_TokenTypes[5] = { 
        { TokenType::VARIABLE, "^var$" },										// Checks if the value is var
		{ TokenType::ID, "^(?!\\d.*$)[a-zA-Z\\d_]*$" },							// Checks if the value is any [a-Z] character, digit or "_" but does not start with a digit
        { TokenType::NUMBER, "^\\d*$"},											// Checks if the value is only digits
		{ TokenType::FLOAT, "^(?:(\\d*\\.(\\d*)?)|((\\d*)?\\.\\d*))$" },		// Checks if the value is only digits with a decimal point
		{ TokenType::STRING, "^(?:(\".*\")|(\'.*\'))$" }						// Checks if any characters are between single or double quotes
    };

	// Different operator tokens, uses regex in the value to find the tokens
	Token m_OperatorTokenTypes[1] = {
		{ TokenType::OPERATOR, "^=$"}
	};
};

#endif