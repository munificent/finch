#include "Token.h"

namespace Finch
{
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
            case TOKEN_AT:              cout << "at"; break;
            case TOKEN_DOT:             cout << "dot"; break;
            case TOKEN_COMMA:           cout << "comma"; break;
            case TOKEN_SEMICOLON:       cout << "semicolon"; break;
            case TOKEN_ARROW:           cout << "->"; break;
            case TOKEN_EQ:              cout << "="; break;
            case TOKEN_SELF:            cout << "self"; break;
            case TOKEN_UNDEFINED:       cout << "undefined"; break;
            case TOKEN_DEF:             cout << "def"; break;
            case TOKEN_OBJ:             cout << "obj"; break;
            case TOKEN_RETURN:          cout << "return"; break;
            case TOKEN_VAR:             cout << "var"; break;

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
                
            case TOKEN_IGNORE_LINE:     cout << "ignore line"; break;
            case TOKEN_EOF:             cout << "eof"; break;
            
            default:
                cout << token.Text();
                break;
        }

        return cout;
    }        
}

