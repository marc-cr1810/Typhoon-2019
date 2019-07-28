#ifndef Ty_TOKENS_H
#define Ty_TOKENS_H

#include "Token.h"

// Different token types, uses regex in the value to find the tokens
const Token TokenTypes[9] = {
	{ TokenType::END, "^\\n$" },
	{ TokenType::STATEMENT, "^var$" },										// Checks if the value is "var"
	{ TokenType::STATEMENT, "^if$" },										// Checks if the value is "if"
	{ TokenType::STATEMENT, "^else$" },										// Checks if the value is "else"
	{ TokenType::NUMBER, "^-?\\d*$"},										// Checks if the value is only digits
	{ TokenType::FLOAT, "^-?(?:(\\d*\\.(\\d*)?)|((\\d*)?\\.\\d*))$" },		// Checks if the value is only digits with a decimal point
	{ TokenType::STRING, "^(?:(\".*\")|(\'.*\'))$" },						// Checks if any characters are between single or double quotes
	{ TokenType::BOOL, "^(?:([T|t]rue)|([F|f]alse))$" },					// Checks if the value is "True", "true" or "False", "false"
	{ TokenType::NAME, "^(?!\\d.*$)[a-zA-Z\\d_]*$" }							// Checks if the value is any [a-Z] character, digit or "_" but does not start with a digit
};

// Different operator tokens, uses regex in the value to find the tokens
const OperatorToken OperatorTokenTypes[16] = {
	{ TokenType::START, "^\\t*$", OperatorType::UNKNOWN },
	{ TokenType::OPERATOR, "^=$", OperatorType::EQUAL },					// Set
	{ TokenType::OPERATOR, "^==$", OperatorType::EQUAL_TO },				// Equal to
	{ TokenType::OPERATOR, "^!=$", OperatorType::NOT_EQUAL_TO },			// Not equal to
	{ TokenType::OPERATOR, "^\\+$", OperatorType::ADD },					// Addition
	{ TokenType::OPERATOR, "^-$", OperatorType::SUBTRACT },					// Subtraction
	{ TokenType::OPERATOR, "^\\*$", OperatorType::MULTIPLY },				// Multiplication
	{ TokenType::OPERATOR, "^\\/$", OperatorType::DIVIDE },					// Division
	{ TokenType::OPERATOR, "^\\^$", OperatorType::POWER },					// Power
	{ TokenType::OPERATOR, "^>$", OperatorType::GREATER_THAN },				// Greater than
	{ TokenType::OPERATOR, "^<$", OperatorType::LESS_THAN },				// Less than
	{ TokenType::OPERATOR, "^>=$", OperatorType::GREATER_THAN_EQUAL_TO },	// Greater than or equal to
	{ TokenType::OPERATOR, "^<=$", OperatorType::LESS_THAN_EQUAL_TO },		// Less than or equal to
	{ TokenType::OPERATOR, "^\\($", OperatorType::LEFT_BRACKET },			// Left bracket
	{ TokenType::OPERATOR, "^\\)$", OperatorType::RIGHT_BRACKET },			// Right bracket
	{ TokenType::OPERATOR, "^:$", OperatorType::SEMICOLON }					// Semicolon
};

#endif