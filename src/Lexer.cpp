#include <iostream> // for debugging

#include "Lexer.h"

namespace Finch
{
    void Lexer::Process(const char * line)
    {
        mLine = line;
        mIndex = 0;
        
        while (true)
        {
            char c = line[mIndex];
            
            switch (mState)
            {
                case LEX_DEFAULT:
                    if      (c == '(')      SingleToken(TOKEN_LEFT_PAREN);
                    else if (c == ')')      SingleToken(TOKEN_RIGHT_PAREN);
                    else if (c == '[')      SingleToken(TOKEN_LEFT_BRACKET);
                    else if (c == ']')      SingleToken(TOKEN_RIGHT_BRACKET);
                    else if (c == '{')      SingleToken(TOKEN_LEFT_BRACE);
                    else if (c == '}')      SingleToken(TOKEN_RIGHT_BRACE);
                    else if (c == ':')      SingleToken(TOKEN_COLON);
                    
                    else if (IsAlpha(c))    StartToken(LEX_IN_NAME);
                    else if (IsOperator(c)) StartToken(LEX_IN_OPERATOR);
                    else mIndex++; // ignore other characters
                    break;
                    
                case LEX_IN_NAME:
                    NameToken(IsAlpha(c) || IsDigit(c) || IsOperator(c),
                              c == ':', TOKEN_NAME);
                    break;
                    
                case LEX_IN_OPERATOR:
                    NameToken(IsAlpha(c) || IsDigit(c) || IsOperator(c),
                              false, TOKEN_OPERATOR);
                    break;
            }
            
            if (c == '\0') break;
        }
        
        mState = LEX_DEFAULT;
    }
    
    void Lexer::SingleToken(TokenType type)
    {
        mTokenStart = mIndex;
        mIndex++;
        Emit(type);
    }
    
    void Lexer::StartToken(State state)
    {
        mTokenStart = mIndex;
        mState = state;
        mIndex++;
    }

    void Lexer::NameToken(bool condition, bool isKeyword, TokenType type)
    {
        if (condition)
        {
            mIndex++;
        }
        else if (isKeyword)
        {
            Emit(TOKEN_KEYWORD);
            // consume the colon
            mIndex++;
            mState = LEX_DEFAULT;
        }
        else
        {
            Emit(type);
            mState = LEX_DEFAULT;
        }
    }

    void Lexer::Emit(TokenType type)
    {
        // pull out the token text
        char text[256];
        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
        text[mIndex - mTokenStart] = '\0';
        
        //### bob: temp
        Token t(type, text);
        std::cout << t << std::endl;
    }
    
    bool Lexer::IsAlpha(char c) const
    {
        return (c == '_') ||
               ((c >= 'a') && (c <= 'z')) ||
               ((c >= 'A') && (c <= 'Z'));
    }
    
    bool Lexer::IsDigit(char c) const
    {
        return (c >= '0') && (c <= '9');
    }
    
    bool Lexer::IsOperator(char c) const
    {
        return (c != '\0') &&
               (strchr("-+=\\/<>?|~!@#$%^&*", c) != NULL);
    }

    bool Lexer::IsSpace(char c) const
    {
        return c == ' ';
    }
    
    bool Lexer::IsNull(char c) const
    {
        return c == '\0';
    }
}