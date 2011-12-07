#include <iostream> // for debugging
#include <cstdlib>

#include "Lexer.h"
#include "IErrorReporter.h"
#include "ILineReader.h"

namespace Finch
{
    bool Lexer::IsInfinite() const
    {
        return mReader.IsInfinite();
    }
    
    Token Lexer::ReadToken()
    {
        if (IsDone()) return Token(TOKEN_EOF);
        
        if (mNeedsLine)
        {
            if (mReader.EndOfLines()) return Token(TOKEN_EOF);
            
            mLine = mReader.NextLine();
            mPos = 0;
            mStart = 0;
            mNeedsLine = false;
        }
        
        while (!IsDone() && IsWhitespace(Peek())) Advance();
        
        if (IsDone()) return Token(TOKEN_EOF);
        
        mStart = mPos;
        
        char c = Peek();
        switch (c)
        {
            case '\0':
                // Skip the rest of the line.
                mNeedsLine = true;
                // But not the line at the end.
                return Token(TOKEN_LINE);
                
            case '(': return SingleToken(TOKEN_LEFT_PAREN);
            case ')': return SingleToken(TOKEN_RIGHT_PAREN);
            case '[': return SingleToken(TOKEN_LEFT_BRACKET);
            case ']': return SingleToken(TOKEN_RIGHT_BRACKET);
            case '{': return SingleToken(TOKEN_LEFT_BRACE);
            case '}': return SingleToken(TOKEN_RIGHT_BRACE);
            case ',': return SingleToken(TOKEN_LINE);
            case '.': return SingleToken(TOKEN_DOT);
            case ';': return SingleToken(TOKEN_SEMICOLON);
            case '\\': return SingleToken(TOKEN_IGNORE_LINE);
            case '|': return SingleToken(TOKEN_PIPE);
            
            case ':':
                Advance();
                if (Peek() == ':')
                {
                    // "::".
                    Advance();
                    return Token(TOKEN_BIND);
                }

                // Just a ":" by itself.
                return Token(TOKEN_KEYWORD);
            
            case '-':
                Advance();
                if (IsDigit(Peek())) return ReadNumber();
                return ReadOperator();
            
            case '/':
                Advance();
                if (Peek() == '/')
                {
                    // Line comment.
                    mNeedsLine = true;
                    return Token(TOKEN_LINE);
                }
                else
                {
                    return ReadOperator();
                }

            
            case '"': return ReadString();
                
            default:
                if (IsDigit(c)) return ReadNumber();
                if (IsAlpha(c)) return ReadName();
                if (IsOperator(c)) return ReadOperator();
        }

        // If we got here, we don't know what it is. Just eat it so we
        // don't get stuck.
        Advance();
        return Token(TOKEN_ERROR, String::Format(
                "Unrecognized character \"%c\".", c));
    }
    
    bool Lexer::IsDone() const
    {
        return mNeedsLine && mReader.EndOfLines();
    }
    
    bool Lexer::IsWhitespace(char c) const
    {
        return (c == ' ') || (c == '\t');
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
               (strchr("-+=/<>?~!@#$%^&*", c) != NULL);
    }
    
    char Lexer::Peek(int ahead) const
    {
        if (mPos + ahead >= mLine.Length()) return '\0';
        return mLine[mPos + ahead];
    }
    
    char Lexer::Advance()
    {
        char c = Peek();
        mPos++;
        return c;
    }
        
    Token Lexer::SingleToken(TokenType type)
    {
        Advance();
        return Token(type);
    }
    
    Token Lexer::ReadString()
    {
        Advance();
        
        String text;
        while (true)
        {
            if (IsDone()) return Token(TOKEN_ERROR, "Unterminated string.");
            
            char c = Advance();
            if (c == '"') return Token(TOKEN_STRING, text);
            
            // An escape sequence.
            if (c == '\\')
            {
                if (IsDone()) return Token(TOKEN_ERROR,
                        "Unterminated string escape.");
                
                char e = Advance();
                switch (e)
                {
                    case 'n': text += "\n"; break;
                    case '"': text += "\""; break;
                    case '\\': text += "\\"; break;
                    default:
                        return Token(TOKEN_ERROR, String::Format(
                                "Unrecognized escape sequence \"%c\".", e));
                }
            }
            else
            {
                // Normal character.
                text += c;
            }
        }
    }
    
    Token Lexer::ReadNumber()
    {
        Advance();
        while (IsDigit(Peek())) Advance();
        
        // Read the fractional part, if any.
        if (Peek() == '.')
        {
            Advance();
            while (IsDigit(Peek())) Advance();
        }

        String text = mLine.Substring(mStart, mPos - mStart);
        double number = atof(text.CString());
        return Token(TOKEN_NUMBER, number);
    }
    
    Token Lexer::ReadName()
    {
        while (IsOperator(Peek()) || IsAlpha(Peek()) || IsDigit(Peek()))
        {
            // Line comments take priority over names.
            if ((Peek() == '/') && (Peek(1) == '/')) break;
            
            Advance();
        }
        
        // If it ends in ":", it's a keyword.
        TokenType type = TOKEN_NAME;
        if (Peek() == ':')
        {
            Advance();
            type = TOKEN_KEYWORD;
        }
        
        String name = mLine.Substring(mStart, mPos - mStart);
        
        if (name == "break") return Token(TOKEN_BREAK);
        if (name == "return") return Token(TOKEN_RETURN);
        if (name == "self") return Token(TOKEN_SELF);
        if (name == "undefined") return Token(TOKEN_UNDEFINED);
        
        return Token(type, name);
    }
    
    Token Lexer::ReadOperator()
    {
        while (IsOperator(Peek()))
        {
            // Line comments take priority over names.
            if ((Peek() == '/') && (Peek(1) == '/')) break;
            
            Advance();
        }
        
        // A mixture of operator characters and letters is a name.
        if (IsAlpha(Peek())) return ReadName();
        
        String name = mLine.Substring(mStart, mPos - mStart);
        
        if (name == "<-") return Token(TOKEN_ARROW);
        if (name == "<--") return Token(TOKEN_LONG_ARROW);
        
        return Token(TOKEN_OPERATOR, name);
    }
}
