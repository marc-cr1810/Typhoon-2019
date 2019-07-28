#ifndef Ty_TOKEN_H
#define Ty_TOKEN_H

#include "../Port.h"

enum TokenType
{
	UNNOWN = -1,
	START,
	END,
	NAME,
	NUMBER,
	FLOAT,
	STRING,
	BOOL,
	OPERATOR,
	STATEMENT
};

enum OperatorType
{
	UNKNOWN = -1,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	POWER,
	LESS_THAN,
	GREATER_THAN,
	EQUAL,
	EQUAL_TO,
	NOT_EQUAL_TO,
	LESS_THAN_EQUAL_TO,
	GREATER_THAN_EQUAL_TO,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	SEMICOLON
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

	OperatorToken(TokenType type, Ty_string_t value, OperatorType opType)
		: Type(type), Value(value), OpType(opType)
	{}
};

#endif