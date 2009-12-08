#include <iostream>

#include "Parser.h"

namespace Finch
{
    void Parser::StartLine(const char * line)
    {
        mLexer.StartLine(line);
        mCurrent.Clear();
    }

    bool Parser::CurrentIs(TokenType type)
    {
        if (mCurrent.IsNull()) mCurrent = mLexer.ReadToken();
        
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
        if (mCurrent.IsNull()) mCurrent = mLexer.ReadToken();
        
        Ref<Token> read = mCurrent;
        mCurrent.Clear();

        return read;
    }
}