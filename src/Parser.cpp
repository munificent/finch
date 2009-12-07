#include <iostream>
#include <memory>

#include "Parser.h"

namespace Finch
{
    using std::auto_ptr;
    
    void Parser::StartLine(const char * line)
    {
        mLexer.StartLine(line);
    }

    bool Parser::CurrentIs(TokenType type)
    {
        // read as needed
        if (mCurrent.get() == NULL)
        {
            Consume();
        }
        
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
    
    const Token & Parser::Consume()
    {
        mCurrent = mLexer.ReadToken();
        return *mCurrent;
    }
    
    const Token & Parser::Consume(TokenType type)
    {
        //### bob: need to check that current is right type!
        return Consume();
    }
}