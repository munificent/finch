#include <iostream> // for debugging

#include "Lexer.h"

namespace Finch
{
    using std::auto_ptr;
    
    void Lexer::StartLine(const char * line)
    {
        mLine = line;
        mIndex = 0;
        mState = LEX_DEFAULT;
    }
    
    auto_ptr<Token> Lexer::ReadToken()
    {
        auto_ptr<Token> token;
        
        while (token.get() == NULL)
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
                    else if (c == ':')      token = SingleToken(TOKEN_COLON);
                    
                    else if (IsAlpha(c))    StartToken(LEX_IN_NAME);
                    else if (IsOperator(c)) StartToken(LEX_IN_OPERATOR);
                    else mIndex++; // ignore other characters
                    break;
                    
                case LEX_IN_NAME:
                    token = NameToken(IsAlpha(c) || IsDigit(c) || IsOperator(c),
                                      c == ':', TOKEN_NAME);
                    break;
                    
                case LEX_IN_OPERATOR:
                    token = NameToken(IsAlpha(c) || IsDigit(c) || IsOperator(c),
                                      false, TOKEN_OPERATOR);
                    break;
                    
                case LEX_AT_END:
                    token = auto_ptr<Token>(new Token(TOKEN_EOF, ""));
                    break;
            }
            
            // after processing the null, we're done
            if (c == '\0') mState = LEX_AT_END;
        }
        
        std::cout << "lex " << *token << std::endl;
        
        return token;
    }
    
    /*
    void Lexer::Process(const char * line)
    {
        StartLine(line);
        
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
    */
    
    auto_ptr<Token> Lexer::SingleToken(TokenType type)
    {
        mTokenStart = mIndex;
        mIndex++;
        return Emit(type);
    }
    
    void Lexer::StartToken(State state)
    {
        mTokenStart = mIndex;
        mState = state;
        mIndex++;
    }

    auto_ptr<Token> Lexer::NameToken(bool condition, bool isKeyword, TokenType type)
    {
        auto_ptr<Token> token;
        
        if (condition)
        {
            mIndex++;
        }
        else if (isKeyword)
        {
            token = Emit(TOKEN_KEYWORD);
            // consume the colon
            mIndex++;
            mState = LEX_DEFAULT;
        }
        else
        {
            token = Emit(type);
            mState = LEX_DEFAULT;
        }
        
        return token;
    }

    auto_ptr<Token> Lexer::Emit(TokenType type)
    {
        // pull out the token text
        char text[256];
        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
        text[mIndex - mTokenStart] = '\0';
        
        return auto_ptr<Token>(new Token(type, text));
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