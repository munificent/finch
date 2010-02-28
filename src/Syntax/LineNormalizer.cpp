#include <iostream> // for debugging
#include <cstdlib>

#include "LineNormalizer.h"

namespace Finch
{
    Token LineNormalizer::ReadToken()
    {
        Token token;
        
        while (token.Type() == TOKEN_NONE)
        {
            token = mTokens.ReadToken();
            
            switch (token.Type())
            {
                case TOKEN_LINE:
                    if (mEatNewlines)
                    {
                        // discard any lines
                        token = Token();
                    }
                    else
                    {
                        // discard newlines after first one
                        mEatNewlines = true;
                    }
                    break;
                    
                case TOKEN_IGNORE_LINE:
                    // eat the ignore token
                    token = Token();
                    
                    // and newlines after it
                    mEatNewlines = true;
                    break;

                // discard newlines after token that can't end expression
                case TOKEN_KEYWORD:
                case TOKEN_OPERATOR:
                case TOKEN_PIPE:
                case TOKEN_ARROW:
                case TOKEN_LONG_ARROW:

                // discard newlines after open group
                case TOKEN_LEFT_PAREN:
                case TOKEN_LEFT_BRACKET:
                case TOKEN_LEFT_BRACE:
                    mEatNewlines = true;
                    break;
                    
                default:
                    mEatNewlines = false;
                    break;
            }
        }
        
        //std::cout << "norm " << *token << std::endl;
        
        return token;
    }
}