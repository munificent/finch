#include <iostream> // for debugging
#include <cstdlib>

#include "LineNormalizer.h"

namespace Finch
{
    Ref<Token> LineNormalizer::ReadToken()
    {
        Ref<Token> token;
        
        while (token.IsNull())
        {
            token = mTokens->ReadToken();
            
            bool isLineToken = token->Type() == TOKEN_LINE;
            
            switch (token->Type())
            {
                case TOKEN_LINE:
                    switch (mState)
                    {
                        case NORMALIZE_DEFAULT:
                            // discard newlines after first one
                            mState = NORMALIZE_DISCARD;
                            break;
                            
                        case NORMALIZE_DISCARD:
                            // discard any lines
                            token.Clear();
                            break;
                            
                        case NORMALIZE_DISCARD_ONE:
                            // discard one line
                            token.Clear();
                            mState = NORMALIZE_DEFAULT;
                            break;
                    }
                    break;

                // discard newlines after token that can't end expression
                case TOKEN_KEYWORD:
                case TOKEN_OPERATOR:
                case TOKEN_PIPE:
                case TOKEN_DEF:
                case TOKEN_SET:
                // discard newlines after open group
                case TOKEN_LEFT_PAREN:
                case TOKEN_LEFT_BRACKET:
                case TOKEN_LEFT_BRACE:
                    mState = NORMALIZE_DISCARD;
                    break;
                    
                // discard one newline after close brace on its own line
                case TOKEN_RIGHT_BRACE:
                    if (mNewLine)
                    {
                        mState = NORMALIZE_DISCARD_ONE;
                    }
                    break;
                    
                default:
                    mState = NORMALIZE_DEFAULT;
                    break;
            }
            
            // keep track of what the previous token was
            mNewLine = isLineToken;
        }
        
        return token;
    }
}