#include <iostream> // for debugging
#include <cstdlib>

#include "Lexer.h"

namespace Finch
{
    void Lexer::StartLine(const char * line)
    {
        mLine = line;
        mIndex = 0;
        mState = LEX_DEFAULT;
    }
    
    Ref<Token> Lexer::ReadToken()
    {
        Ref<Token> token;
        
        while (token.IsNull())
        {
            char c = mLine[mIndex];
            
            switch (mState)
            {
                case LEX_DEFAULT:
                    if      (c == '(')      token = SingleToken(TOKEN_LEFT_PAREN);
                    else if (c == ')')      token = SingleToken(TOKEN_RIGHT_PAREN);
                    else if (c == '[')      token = SingleToken(TOKEN_LEFT_BRACKET);
                    else if (c == ']')      token = SingleToken(TOKEN_RIGHT_BRACKET);
                    else if (c == '{')      token = SingleToken(TOKEN_LEFT_BRACE);
                    else if (c == '}')      token = SingleToken(TOKEN_RIGHT_BRACE);
                    else if (c == '.')      token = SingleToken(TOKEN_DOT);
                    else if (c == ':')      token = SingleToken(TOKEN_COLON);
                    else if (c == ';')      token = SingleToken(TOKEN_SEMICOLON);
                    else if (c == '|')      token = SingleToken(TOKEN_PIPE);
                    
                    else if (IsDigit(c))    StartToken(LEX_IN_NUMBER);
                    else if (IsAlpha(c))    StartToken(LEX_IN_NAME);
                    else if (IsOperator(c)) StartToken(LEX_IN_OPERATOR);
                    else if (c == '`')      StartToken(LEX_IN_SYMBOL, true);
                    
                    else mIndex++; // ignore other characters
                    break;
                    
                case LEX_IN_NUMBER:
                    if (IsDigit(c))
                    {
                        mIndex++;
                    }
                    else
                    {
                        token = EmitNumber(TOKEN_NUMBER);
                        mState = LEX_DEFAULT;
                    }
                    break;
                    
                case LEX_IN_NAME:
                    token = TextToken(IsAlpha(c) || IsDigit(c) || IsOperator(c),
                                      c == ':', TOKEN_NAME);
                    break;
                    
                case LEX_IN_OPERATOR:
                    token = TextToken(IsAlpha(c) || IsDigit(c) || IsOperator(c),
                                      false, TOKEN_OPERATOR);
                    break;
                    
                case LEX_IN_SYMBOL:
                    token = TextToken(IsAlpha(c) || IsDigit(c) ||
                                      IsOperator(c) || (c == ':'),
                                      false, TOKEN_SYMBOL);
                    break;
                    
                case LEX_AT_END:
                    token = Token::New(TOKEN_EOF);
                    break;
            }
            
            // after processing the null, we're done
            if (c == '\0') mState = LEX_AT_END;
        }
        
        // std::cout << "lex " << *token << std::endl;
        
        return token;
    }
    
    Ref<Token> Lexer::SingleToken(TokenType type)
    {
        mIndex++;
        return Token::New(type);
    }
    
    void Lexer::StartToken(State state, bool skipFirstChar)
    {
        mTokenStart = mIndex;
        mState = state;
        mIndex++;
        
        if (skipFirstChar) mTokenStart++;
    }
    
    Ref<Token> Lexer::TextToken(bool condition, bool isKeyword, TokenType type)
    {
        Ref<Token> token;
        
        if (condition)
        {
            mIndex++;
        }
        else if (isKeyword)
        {
            // consume the colon
            mIndex++;
            
            token = EmitText(TOKEN_KEYWORD);
            mState = LEX_DEFAULT;
        }
        else
        {
            token = EmitText(type);
            mState = LEX_DEFAULT;
        }
        
        return token;
    }
    
    Ref<Token> Lexer::EmitNumber(TokenType type)
    {
        // pull out the token text
        char text[256];
        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
        text[mIndex - mTokenStart] = '\0';
        
        double number = atof(text);
        
        return Token::New(type, number);
    }
    
    Ref<Token> Lexer::EmitText(TokenType type)
    {
        // pull out the token text
        char text[256];
        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
        text[mIndex - mTokenStart] = '\0';
        
        String name = text;
        
        // see if it's a reserved word
        if (name == "def") return Token::New(TOKEN_DEF);
        if (name == "set") return Token::New(TOKEN_SET);
        
        return Token::New(type, name);
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
               (strchr("-+=\\/<>?~!@#$%^&*", c) != NULL);
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