#ifndef Ty_TOKENS_H
#define Ty_TOKENS_H

#include "Token.h"

// Different token types, uses regex in the value to find the tokens
const Token TokenTypes[10] = {
	{ TokenType::END, "^\\n$" },
	{ TokenType::FUNCTION_CALL, "^func$" },										// Checks if the value is "func"
	{ TokenType::DECLARATION, "^var$" },										// Checks if the value is "var"
	{ TokenType::STATEMENT, "^if$" },										// Checks if the value is "if"
	{ TokenType::STATEMENT, "^else$" },										// Checks if the value is "else"
	{ TokenType::NUMBER, "^-?(0x)?\\d*$"},										// Checks if the value is only digits
	{ TokenType::FLOAT, "^-?(?:(\\d*\\.(\\d*)?)|((\\d*)?\\.\\d*))$" },		// Checks if the value is only digits with a decimal point
	{ TokenType::STRING, "^(?:(\".*\")|(\'.*\'))$" },						// Checks if any characters are between single or double quotes
	{ TokenType::BOOL, "^(?:([T|t]rue)|([F|f]alse))$" },					// Checks if the value is "True", "true" or "False", "false"
	{ TokenType::NAME, "^(?!\\d.*$)[a-zA-Z\\d_]*$" }						// Checks if the value is any [a-Z] character, digit or "_" but does not start with a digit
};

// Different operator tokens, uses regex in the value to find the tokens
const OperatorToken OperatorTokenTypes[16] = {
	{ TokenType::START, "^\\t*$", OperatorType::UNKNOWN, OperatorCategory::CATEGORY_UNKNOWN },
	{ TokenType::OPERATOR, "^=$", OperatorType::EQUAL, OperatorCategory::ASSIGNMENT },					// Set
	{ TokenType::OPERATOR, "^==$", OperatorType::EQUAL_TO, OperatorCategory::EQUALITY },				// Equal to
	{ TokenType::OPERATOR, "^!=$", OperatorType::NOT_EQUAL_TO, OperatorCategory::EQUALITY },			// Not equal to
	{ TokenType::OPERATOR, "^\\+$", OperatorType::ADD, OperatorCategory::ADDITIVE },					// Addition
	{ TokenType::OPERATOR, "^-$", OperatorType::SUBTRACT, OperatorCategory::ADDITIVE },					// Subtraction
	{ TokenType::OPERATOR, "^\\*$", OperatorType::MULTIPLY, OperatorCategory::MULTIPLICATIVE },			// Multiplication
	{ TokenType::OPERATOR, "^\\/$", OperatorType::DIVIDE, OperatorCategory::MULTIPLICATIVE },			// Division
	{ TokenType::OPERATOR, "^>$", OperatorType::GREATER_THAN, OperatorCategory::RELATIONAL },			// Greater than
	{ TokenType::OPERATOR, "^<$", OperatorType::LESS_THAN, OperatorCategory::RELATIONAL },				// Less than
	{ TokenType::OPERATOR, "^>=$", OperatorType::GREATER_THAN_EQUAL_TO, OperatorCategory::RELATIONAL },	// Greater than or equal to
	{ TokenType::OPERATOR, "^<=$", OperatorType::LESS_THAN_EQUAL_TO, OperatorCategory::RELATIONAL },	// Less than or equal to
	{ TokenType::OPERATOR, "^\\($", OperatorType::LEFT_BRACKET, OperatorCategory::POSTFIX },			// Left bracket
	{ TokenType::OPERATOR, "^\\)$", OperatorType::RIGHT_BRACKET, OperatorCategory::POSTFIX },			// Right bracket
	{ TokenType::OPERATOR, "^,$", OperatorType::COMMA, OperatorCategory::CATEGORY_UNKNOWN },			// Right bracket
	{ TokenType::OPERATOR_SPECIAL, "^:$", OperatorType::SEMICOLON, OperatorCategory::CATEGORY_UNKNOWN }	// Semicolon
};

static OperatorToken TokenToOperatorToken(Token token)
{
	for (OperatorToken opToken : OperatorTokenTypes)
	{
		if (std::regex_match(token.Value, std::regex(opToken.Value)))
			return opToken;
	}
	return { TokenType::UNKNOWN_TOKEN, "", OperatorType::UNKNOWN, OperatorCategory::CATEGORY_UNKNOWN };
}

#endif