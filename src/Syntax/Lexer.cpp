#include <iostream> // for debugging
#include <cstdlib>
#include <cstring>

#include "Lexer.h"
#include "IErrorReporter.h"
#include "ILineReader.h"

namespace Finch
{
    bool Lexer::IsInfinite() const
    {
        return mReader.IsInfinite();
    }
    
    Ref<Token> Lexer::ReadToken()
    {
        while (true)
        {
            if (IsDone()) return Ref<Token>(new Token(TOKEN_EOF));
            
            if (mNeedsLine)
            {
                AdvanceLine();
                continue;
            }
            
            mStart = mPos;
            
            char c = Peek();
            switch (c)
            {
                case ' ':
                case '\t':
                    // Skip whitespace.
                    while (IsWhitespace(Peek())) Advance();
                    break;
                    
                case '\0':
                    // End of the line.
                    mNeedsLine = true;
                    return Ref<Token>(new Token(TOKEN_LINE));
                    
                case '(': return SingleToken(TOKEN_LEFT_PAREN);
                case ')': return SingleToken(TOKEN_RIGHT_PAREN);
                case '[': return SingleToken(TOKEN_LEFT_BRACKET);
                case ']': return SingleToken(TOKEN_RIGHT_BRACKET);
                case '{': return SingleToken(TOKEN_LEFT_BRACE);
                case '}': return SingleToken(TOKEN_RIGHT_BRACE);
                case ',': return SingleToken(TOKEN_LINE);
                case '@': return SingleToken(TOKEN_AT);
                case '.': return SingleToken(TOKEN_DOT);
                case '#': return SingleToken(TOKEN_HASH);
                case ';': return SingleToken(TOKEN_SEMICOLON);
                case '\\': return SingleToken(TOKEN_IGNORE_LINE);
                case '|': return SingleToken(TOKEN_PIPE);
                
                case ':':
                    Advance();
                    if (Peek() == ':')
                    {
                        // "::".
                        Advance();
                        return Ref<Token>(new Token(TOKEN_BIND));
                    }

                    // Just a ":" by itself.
                    return Ref<Token>(new Token(TOKEN_KEYWORD, ":"));
                
                case '-':
                    Advance();
                    if (IsDigit(Peek())) return ReadNumber();
                    return ReadOperator();
                
                case '/':
                    Advance();
                    if (Peek() == '/')
                    {
                        // Line comment, so ignore the rest of the line and
                        // emit the line token.
                        mNeedsLine = true;
                        return Ref<Token>(new Token(TOKEN_LINE));
                    }
                    else if (Peek() == '*')
                    {
                        SkipBlockComment();
                    }
                    else
                    {
                        return ReadOperator();
                    }
                    break;

                case '"': return ReadString();
                    
                default:
                    if (IsDigit(c)) return ReadNumber();
                    if (IsAlpha(c)) return ReadName();
                    if (IsOperator(c)) return ReadOperator();
                    
                    // If we got here, we don't know what it is. Just eat it so
                    // we don't get stuck.
                    Advance();
                    return Ref<Token>(new Token(TOKEN_ERROR, String::Format(
                        "Unrecognized character \"%c\".", c)));
            }
        }
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
               (strchr("-+=/<>?~!$%^&*", c) != NULL);
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
    
    void Lexer::SkipBlockComment()
    {
        Advance();
        Advance();
        
        int nesting = 1;
        
        while (nesting > 0)
        {
            // TODO(bob): Unterminated comment. Should return error.
            if (IsDone()) return;
            
            if ((Peek() == '/') && (Peek(1) == '*'))
            {
                Advance();
                Advance();
                nesting++;
            }
            else if ((Peek() == '*') && (Peek(1) == '/'))
            {
                Advance();
                Advance();
                nesting--;
            }
            else if (Peek() == '\0')
            {
                AdvanceLine();
            }
            else
            {
                Advance();
            }
        }
    }
    
    Ref<Token> Lexer::SingleToken(TokenType type)
    {
        Advance();
        return Ref<Token>(new Token(type));
    }
    
    Ref<Token> Lexer::ReadString()
    {
        Advance();
        
        String text;
        while (true)
        {
            if (IsDone()) return Ref<Token>(new Token(TOKEN_ERROR, "Unterminated string."));
            
            char c = Advance();
            if (c == '"') return Ref<Token>(new Token(TOKEN_STRING, text));
            
            // An escape sequence.
            if (c == '\\')
            {
                if (IsDone()) return Ref<Token>(new Token(TOKEN_ERROR,
                        "Unterminated string escape."));
                
                char e = Advance();
                switch (e)
                {
                    case 'n': text += "\n"; break;
                    case '"': text += "\""; break;
                    case '\\': text += "\\"; break;
                    case 't': text += "\t"; break;
                    default:
                        return Ref<Token>(new Token(TOKEN_ERROR, String::Format(
                                "Unrecognized escape sequence \"%c\".", e)));
                }
            }
            else
            {
                // Normal character.
                text += c;
            }
        }
    }
    
    Ref<Token> Lexer::ReadNumber()
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
        return Ref<Token>(new Token(TOKEN_NUMBER, number));
    }
    
    Ref<Token> Lexer::ReadName()
    {
        while (IsOperator(Peek()) || IsAlpha(Peek()) || IsDigit(Peek()))
        {
            // Comments take priority over names.
            if ((Peek() == '/') && (Peek(1) == '/')) break;
            if ((Peek() == '/') && (Peek(1) == '*')) break;
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
        
        if (name == "return") return Ref<Token>(new Token(TOKEN_RETURN));
        if (name == "self") return Ref<Token>(new Token(TOKEN_SELF));
        if (name == "undefined") return Ref<Token>(new Token(TOKEN_UNDEFINED));
        
        return Ref<Token>(new Token(type, name));
    }
    
    Ref<Token> Lexer::ReadOperator()
    {
        while (IsOperator(Peek()))
        {
            // Comments take priority over names.
            if ((Peek() == '/') && (Peek(1) == '/')) break;
            if ((Peek() == '/') && (Peek(1) == '*')) break;

            Advance();
        }
        
        // A mixture of operator characters and letters is a name.
        if (IsAlpha(Peek())) return ReadName();
        
        String name = mLine.Substring(mStart, mPos - mStart);
        
        if (name == "<-") return Ref<Token>(new Token(TOKEN_ARROW));
        if (name == "->") return Ref<Token>(new Token(TOKEN_RIGHT_ARROW));
        if (name == "=") return Ref<Token>(new Token(TOKEN_EQ));
        
        return Ref<Token>(new Token(TOKEN_OPERATOR, name));
    }
    
    void Lexer::AdvanceLine()
    {
        mLine = mReader.NextLine();
        mPos = 0;
        mStart = 0;
        mNeedsLine = false;
    }
}
