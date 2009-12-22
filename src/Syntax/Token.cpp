#include "Token.h"

namespace Finch
{
    // "hidden" token subclasses
    class NumberToken : public Token
    {
        friend class Token;
        
    public:
        virtual double Number() const { return mNumber; }
        
    private:
        NumberToken(TokenType type, double number)
        :   Token(type),
        mNumber(number)
        {}
        
        double      mNumber;
    };
    
    class StringToken : public Token
    {
        friend class Token;
        
    public:
        virtual String Text() const { return mText; }
        
    private:
        StringToken(TokenType type, const String & text)
        :   Token(type),
        mText(text)
        {}
        
        TokenType   mType;
        String      mText;
        double      mNumber;
    };
    
    // virtual constructors
    Ref<Token> Token::New(TokenType type)
    {
        return Ref<Token>(new Token(type));
    }
    
    Ref<Token> Token::New(TokenType type, const String & text)
    {
        return Ref<Token>(new StringToken(type, text));
    }
    
    Ref<Token> Token::New(TokenType type, double number)
    {
        return Ref<Token>(new NumberToken(type, number));
    }

    std::ostream& operator<<(std::ostream& cout, const Token & token)
    {
        switch (token.Type())
        {                
            case TOKEN_LEFT_PAREN:      cout << "left paren"; break;
            case TOKEN_RIGHT_PAREN:     cout << "right paren"; break;
            case TOKEN_LEFT_BRACKET:    cout << "left bracket"; break;
            case TOKEN_RIGHT_BRACKET:   cout << "right bracket"; break;
            case TOKEN_LEFT_BRACE:      cout << "left brace"; break;
            case TOKEN_RIGHT_BRACE:     cout << "right brace"; break;
            case TOKEN_DOT:             cout << "dot"; break;
            case TOKEN_PIPE:            cout << "pipe"; break;
            case TOKEN_DEF:             cout << "def"; break;
            case TOKEN_SET:             cout << "set"; break;
                
            case TOKEN_NUMBER:
                cout << "number " << token.Number();
                break;
                
            case TOKEN_STRING:
                cout << "string '" << token.Text() << "'";
                break;
                
            case TOKEN_NAME:
                cout << "name '" << token.Text() << "'";
                break;
                
            case TOKEN_OPERATOR:
                cout << "operator '" << token.Text() << "'";
                break;
                
            case TOKEN_KEYWORD:
                cout << "keyword '" << token.Text() << "'";
                break;
                
            case TOKEN_LINE:            cout << "newline"; break;
            case TOKEN_EOF:             cout << "eof"; break;
        }

        return cout;
    }        
}