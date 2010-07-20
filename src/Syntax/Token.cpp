#include "Token.h"

namespace Finch
{
    std::ostream& operator<<(std::ostream& cout, const Token & token)
    {
        switch (token.Type())
        {
            case TOKEN_NONE:            cout << "none"; break;
            
            case TOKEN_LEFT_PAREN:      cout << "left paren"; break;
            case TOKEN_RIGHT_PAREN:     cout << "right paren"; break;
            case TOKEN_LEFT_BRACKET:    cout << "left bracket"; break;
            case TOKEN_RIGHT_BRACKET:   cout << "right bracket"; break;
            case TOKEN_LEFT_BRACE:      cout << "left brace"; break;
            case TOKEN_RIGHT_BRACE:     cout << "right brace"; break;
            case TOKEN_COMMA:           cout << "comma"; break;
            case TOKEN_DOT:             cout << "dot"; break;
            case TOKEN_PIPE:            cout << "pipe"; break;
            case TOKEN_ARROW:           cout << "<-"; break;
            case TOKEN_BIND:            cout << "::"; break;
            case TOKEN_LONG_ARROW:      cout << "<--"; break;
            case TOKEN_SELF:            cout << "self"; break;
            case TOKEN_UNDEFINED:       cout << "undefined"; break;
                
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
            case TOKEN_IGNORE_LINE:     cout << "ignore line"; break;
            case TOKEN_EOF:             cout << "eof"; break;
        }

        return cout;
    }        
}

