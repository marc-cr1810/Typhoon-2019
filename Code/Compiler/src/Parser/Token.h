#ifndef Ty_TOKEN_H
#define Ty_TOKEN_H

#include "../Port.h"

enum TokenType
{
	UNKNOWN_TOKEN = -1,
	START,
	END,
	NAME,
	NUMBER,
	FLOAT,
	STRING,
	BOOL,
	OPERATOR,
	OPERATOR_SPECIAL,
	STATEMENT,
	DECLARATION,
	FUNCTION_CALL
};

enum OperatorType
{
	UNKNOWN = -1,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	LESS_THAN,
	GREATER_THAN,
	EQUAL,
	EQUAL_TO,
	NOT_EQUAL_TO,
	LESS_THAN_EQUAL_TO,
	GREATER_THAN_EQUAL_TO,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	SEMICOLON,
	COMMA
};

enum OperatorCategory
{
	CATEGORY_UNKNOWN = -1,
	ASSIGNMENT,
	CONDITIONAL,
	LOGICAL_OR,
	LOGICAL_AND,
	BITWISE_OR,
	BITWISE_AND,
	EQUALITY,
	RELATIONAL,
	SHIFT,
	ADDITIVE,
	MULTIPLICATIVE,
	UNARY,
	POSTFIX
};

struct Token
{
    TokenType Type;
    Ty_string_t Value;

    Token(TokenType type, Ty_string_t value)
        : Type(type), Value(value)
    { }
};

struct OperatorToken
{
	TokenType Type;
	Ty_string_t Value;
	OperatorType OpType;
	OperatorCategory Category;

	OperatorToken(TokenType type, Ty_string_t value, OperatorType opType, OperatorCategory category)
		: Type(type), Value(value), OpType(opType), Category(category)
	{}
};

#endif