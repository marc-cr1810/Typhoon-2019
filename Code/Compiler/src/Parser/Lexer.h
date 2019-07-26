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

    // Different token types, uses regex in the value to find the tokens
    Token m_TokenTypes[7] = {
		{ TokenType::END, "^\\n$" },
        { TokenType::VARIABLE, "^var$" },										// Checks if the value is var
        { TokenType::NUMBER, "^\\d*$"},											// Checks if the value is only digits
		{ TokenType::FLOAT, "^(?:(\\d*\\.(\\d*)?)|((\\d*)?\\.\\d*))$" },		// Checks if the value is only digits with a decimal point
		{ TokenType::STRING, "^(?:(\".*\")|(\'.*\'))$" },						// Checks if any characters are between single or double quotes
		{ TokenType::BOOL, "^(?:([T|t]rue)|([F|f]alse))$" },					// Checks if the value is "True", "true" or "False", "false"
		{ TokenType::ID, "^(?!\\d.*$)[a-zA-Z\\d_]*$" }							// Checks if the value is any [a-Z] character, digit or "_" but does not start with a digit
    };

	// Different operator tokens, uses regex in the value to find the tokens
	Token m_OperatorTokenTypes[14] = {
		{ TokenType::OPERATOR, "^=$"},			// Set
		{ TokenType::OPERATOR, "^==$"},			// Equal to
		{ TokenType::OPERATOR, "^!=$"},			// Not equal to
		{ TokenType::OPERATOR, "^\\+$"},		// Addition
		{ TokenType::OPERATOR, "^-$"},			// Subtraction
		{ TokenType::OPERATOR, "^\\*$"},		// Multiplication
		{ TokenType::OPERATOR, "^\\/$"},		// Division
		{ TokenType::OPERATOR, "^\\^$"},		// Power
		{ TokenType::OPERATOR, "^>$"},			// Greater than
		{ TokenType::OPERATOR, "^<$"},			// Less than
		{ TokenType::OPERATOR, "^>=$"},			// Greater than or equal to
		{ TokenType::OPERATOR, "^<=$"},			// Less than or equal to
		{ TokenType::OPERATOR, "^\\($"},		// Left bracket
		{ TokenType::OPERATOR, "^\\)$"}			// Right bracket
	};
};

#endif