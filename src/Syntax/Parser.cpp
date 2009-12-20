#include <iostream>

#include "Parser.h"
#include "ILineReader.h"
#include "ITokenSource.h"

namespace Finch
{
    bool Parser::CurrentIs(TokenType type)
    {
        if (mCurrent.IsNull()) mCurrent = mTokens->ReadToken();
        
        return mCurrent->Type() == type;
    }
    
    bool Parser::ConsumeIf(TokenType type)
    {
        if (CurrentIs(type))
        {
            Consume();
            return true;
        }
        else
        {
            return false;
        }
    }
    
    Ref<Token> Parser::Consume()
    {
        if (mCurrent.IsNull()) mCurrent = mTokens->ReadToken();
        
        Ref<Token> read = mCurrent;
        mCurrent.Clear();
        
        return read;
    }
}