#include <iostream> // for debugging
#include <cstdlib>

#include "Lexer.h"
#include "ILineReader.h"

namespace Finch
{
    Ref<Token> Lexer::ReadToken()
    {
        Ref<Token> token;
        
        while (token.IsNull())
        {
            char c = mLine[mIndex];
            
            switch (mState)
            {
                case LEX_NEED_LINE:
                    if (mReader->EndOfLines())
                    {
                        mState = LEX_DONE;
                    }
                    else
                    {
                        mLine = mReader->NextLine();
                        mIndex = 0;
                        mState = LEX_DEFAULT;
                    }
                    break;
                    
                case LEX_DEFAULT:
                    switch (c)
                    {
                        case '(': token = SingleToken(TOKEN_LEFT_PAREN); break;
                        case ')': token = SingleToken(TOKEN_RIGHT_PAREN); break;
                        case '[': token = SingleToken(TOKEN_LEFT_BRACKET); break;
                        case ']': token = SingleToken(TOKEN_RIGHT_BRACKET); break;
                        case '{': token = SingleToken(TOKEN_LEFT_BRACE); break;
                        case '}': token = SingleToken(TOKEN_RIGHT_BRACE); break;
                        case '.': token = SingleToken(TOKEN_DOT); break;
                        case ':': token = SingleToken(TOKEN_KEYWORD); break;
                        case ';': token = SingleToken(TOKEN_LINE); break;
                        case '|': token = SingleToken(TOKEN_PIPE); break;
                            
                        case '-': StartToken(LEX_IN_MINUS); break;
                            
                        case '"': StartToken(LEX_IN_STRING); break;
                            
                        case '\'':
                            mState = LEX_IN_COMMENT;
                            break;
                            
                        case '\0':
                            token = Token::New(TOKEN_LINE);
                            mState = LEX_NEED_LINE;
                            break;
                        
                        case ' ': Consume(); break;
                            
                        default:
                            if (IsDigit(c))         StartToken(LEX_IN_NUMBER);
                            else if (IsAlpha(c))    StartToken(LEX_IN_NAME);
                            else if (IsOperator(c)) StartToken(LEX_IN_OPERATOR);
                            else
                            {
                                //### bob: need lexer error handling here
                                std::cout << "unexpected char '" << c << "'\n";
                                Consume();
                            }
                    }
                    break;
                    
                case LEX_IN_MINUS:
                    if (IsDigit(c))         ChangeToken(LEX_IN_NUMBER);
                    else if (IsOperator(c)) ChangeToken(LEX_IN_OPERATOR);
                    else
                    {
                        token = Token::New(TOKEN_OPERATOR, "-");
                        mState = LEX_DEFAULT;
                    }
                    break;
                    
                case LEX_IN_NUMBER:
                    if (IsDigit(c)) Consume();
                    else
                    {
                        // pull out the token text
                        char text[256];
                        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
                        text[mIndex - mTokenStart] = '\0';
                        
                        double number = atof(text);
                        
                        token = Token::New(TOKEN_NUMBER, number);
                        
                        mState = LEX_DEFAULT;
                    }
                    break;
                    
                case LEX_IN_NAME:
                    if (IsAlpha(c) || IsDigit(c) || IsOperator(c)) Consume();
                    else if (c == ':')
                    {
                        Consume();
                        
                        // pull out the token text
                        char text[256];
                        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
                        text[mIndex - mTokenStart] = '\0';
                        
                        String name = text;
                        
                        token = Token::New(TOKEN_KEYWORD, name);

                        mState = LEX_DEFAULT;
                    }
                    else
                    {
                        // pull out the token text
                        char text[256];
                        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
                        text[mIndex - mTokenStart] = '\0';
                        
                        String name = text;
                        
                        // see if it's a reserved word
                        if (name == "def")        token = Token::New(TOKEN_DEF);
                        else if (name == "undef") token = Token::New(TOKEN_UNDEF);
                        else token = Token::New(TOKEN_NAME, name);
                        
                        mState = LEX_DEFAULT;
                    }
                    break;
                    
                case LEX_IN_OPERATOR:
                    if (IsOperator(c))   Consume();
                    else if (IsAlpha(c)) mState = LEX_IN_NAME;
                    else
                    {
                        // pull out the token text
                        char text[256];
                        strncpy(text, &mLine[mTokenStart], mIndex - mTokenStart);
                        text[mIndex - mTokenStart] = '\0';
                        
                        String name = text;
                        
                        // see if it's a reserved word
                        if (name == "<-") token = Token::New(TOKEN_LEFT_ARROW);
                        else token = Token::New(TOKEN_OPERATOR, name);
                        
                        mState = LEX_DEFAULT;
                    }
                    break;
                    
                case LEX_IN_STRING:
                    if (c == '"')
                    {
                        Consume();
                        
                        char text[256];
                        // skip the surrounding quotes
                        strncpy(text, &mLine[mTokenStart + 1], mIndex - mTokenStart - 2);
                        text[mIndex - mTokenStart - 2] = '\0';
                        
                        token = Token::New(TOKEN_STRING, text);
                        
                        mState = LEX_DEFAULT;
                    }
                    else Consume();
                    break;
                    
                case LEX_IN_COMMENT:
                    if (c == '\0')
                    {
                        token = Token::New(TOKEN_LINE);
                        mState = LEX_NEED_LINE;
                    }
                    else Consume();
                    break;
                    
                case LEX_DONE:
                    token = Token::New(TOKEN_EOF);
                    break;
            }
        }
        
        return token;
    }
    
    void Lexer::Consume()
    {
        mIndex++;
    }
    
    void Lexer::StartLine()
    {
        if (mReader->EndOfLines())
        {
            mState = LEX_DONE;
        }
        else
        {
            mLine = mReader->NextLine();
            mIndex = 0;
            mState = LEX_DEFAULT;
        }
    }
    
    Ref<Token> Lexer::SingleToken(TokenType type)
    {
        mIndex++;
        return Token::New(type);
    }
    
    void Lexer::StartToken(State state)
    {
        mState = state;
        mTokenStart = mIndex;
        Consume();
    }

    void Lexer::ChangeToken(State state)
    {
        Consume();
        mState = state;
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