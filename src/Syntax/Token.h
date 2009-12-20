#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
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
        TOKEN_PIPE,
        
        TOKEN_DEF,
        TOKEN_SET,
        
        TOKEN_NUMBER,
        TOKEN_STRING,
        
        TOKEN_NAME,
        TOKEN_OPERATOR,
        TOKEN_KEYWORD,
        
        TOKEN_LINE,
        TOKEN_EOF
    };

    class Token
    {
    public:
        // virtual constructors
        static Ref<Token> New(TokenType type);
        static Ref<Token> New(TokenType type, const String & text);
        static Ref<Token> New(TokenType type, double number);
        
        TokenType           Type()   const { return mType; }
        virtual String      Text()   const { return ""; }
        virtual double      Number() const { return 0; }
        
    protected:
        Token(TokenType type)
        :   mType(type)
        {}
        
    private:        
        TokenType   mType;
    };
    
    std::ostream& operator<<(std::ostream& cout, const Token & token);
}