#pragma once

#include <iostream>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    enum TokenType
    {
        TOKEN_LEFT_PAREN,
        TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACKET,
        TOKEN_RIGHT_BRACKET,
        TOKEN_LEFT_BRACE,
        TOKEN_RIGHT_BRACE,
        TOKEN_DOT,
        TOKEN_COLON,
        TOKEN_SEMICOLON,
        TOKEN_PIPE,
        
        TOKEN_NUMBER,
        
        TOKEN_NAME,
        TOKEN_OPERATOR,
        TOKEN_KEYWORD,
        
        TOKEN_EOF
    };

    class Token
    {
    public:
        Token(TokenType type)
        :   mType(type),
            mText(""),
            mNumber(0)
        {}
        
        Token(TokenType type, const String & text)
        :   mType(type),
            mText(text),
            mNumber(0)
        {}
        
        Token(TokenType type, double number)
        :   mType(type),
            mText(""),
            mNumber(number)
        {}
        
        TokenType   Type()   const { return mType; }
        String      Text()   const { return mText; }
        double      Number() const { return mNumber; }
        
    private:
        TokenType   mType;
        String      mText;
        double      mNumber;
    };

    std::ostream& operator<<(std::ostream& cout, const Token & token);
}