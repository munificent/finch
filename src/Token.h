#pragma once

#include <iostream>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    enum TokenType
    {
        TOKEN_NAME,
        TOKEN_OPERATOR,
        TOKEN_KEYWORD,
        
        TOKEN_LEFT_PAREN,
        TOKEN_RIGHT_PAREN,
        TOKEN_LEFT_BRACKET,
        TOKEN_RIGHT_BRACKET,
        TOKEN_LEFT_BRACE,
        TOKEN_RIGHT_BRACE,
        TOKEN_COLON,

        TOKEN_EOF
    };

    class Token
    {
    public:
        Token(TokenType type, const String & text)
        :   mType(type),
            mText(text)
        {}
        
        TokenType Type() const { return mType; }
        String    Text() const { return mText; }
        
    private:
        TokenType   mType;
        String      mText;
    };

    std::ostream& operator<<(std::ostream& cout, const Token & token);
}