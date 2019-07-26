#ifndef Ty_TOKEN_H
#define Ty_TOKEN_H

#include "../Port.h"

enum TokenType 
{
    UNNOWN = -1,
    VARIABLE,
    NUMBER
};

struct Token
{
    TokenType Type;
    Ty_string_t Value;

    Token(TokenType type, Ty_string_t value)
        : Type(type), Value(value)
    { }
};

#endif