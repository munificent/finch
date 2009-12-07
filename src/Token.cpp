#include "Token.h"

namespace Finch
{
    std::ostream& operator<<(std::ostream& cout, const Token & token)
    {
        switch (token.Type())
        {
            case TOKEN_NAME:
                cout << "name '" << token.Text() << "'";
                break;
                
            case TOKEN_OPERATOR:
                cout << "operator '" << token.Text() << "'";
                break;
                
            case TOKEN_KEYWORD:
                cout << "keyword '" << token.Text() << "'";
                break;
                
            case TOKEN_LEFT_PAREN:      cout << "left paren"; break;
            case TOKEN_RIGHT_PAREN:     cout << "right paren"; break;
            case TOKEN_LEFT_BRACKET:    cout << "left bracket"; break;
            case TOKEN_RIGHT_BRACKET:   cout << "right bracket"; break;
            case TOKEN_LEFT_BRACE:      cout << "left brace"; break;
            case TOKEN_RIGHT_BRACE:     cout << "right brace"; break;
            case TOKEN_COLON:           cout << "colon"; break;
            case TOKEN_EOF:             cout << "eof"; break;
        }

        return cout;
    }        
}