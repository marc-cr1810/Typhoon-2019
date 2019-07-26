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
    char m_SeperatorChars[1] = { ' ' };

    // Different token types, uses regex in the value to find the tokens
    Token m_TokenTypes[2] = { 
        { TokenType::VARIABLE, "^var$" },
        { TokenType::NUMBER, "^\\d*$"}
    };
};

#endif