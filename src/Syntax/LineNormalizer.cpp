#include "LineNormalizer.h"

namespace Finch
{
    bool LineNormalizer::IsInfinite() const
    {
        return mTokens.IsInfinite();
    }
    
    Ref<Token> LineNormalizer::ReadToken()
    {
        Ref<Token> token;
        
        while (token.IsNull())
        {
            token = mTokens.ReadToken();
            
            switch (token->Type())
            {
                case TOKEN_SEMICOLON:
                    if (mEatNewlines)
                    {
                        // discard any lines
                        token.Clear();
                    }
                    else
                    {
                        // discard newlines after first one
                        mEatNewlines = true;
                    }
                    break;
                    
                case TOKEN_IGNORE_LINE:
                    // eat the ignore token
                    token.Clear();
                    
                    // and newlines after it
                    mEatNewlines = true;
                    break;

                // discard newlines after token that can't end expression
                case TOKEN_COMMA:
                case TOKEN_KEYWORD:
                case TOKEN_OPERATOR:
                case TOKEN_EQ:
                case TOKEN_ARROW:

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
        
        return token;
    }
}

