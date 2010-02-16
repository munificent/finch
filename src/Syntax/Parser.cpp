#include <iostream>

#include "Parser.h"
#include "ILineReader.h"
#include "ITokenSource.h"

namespace Finch
{
    bool Parser::LookAhead(TokenType type)
    {
        FillLookAhead(1);
        
        return mRead[0]->Type() == type;
    }
    
    bool Parser::LookAhead(TokenType current, TokenType next)
    {
        FillLookAhead(2);

        return (mRead[0]->Type() == current) &&
               (mRead[1]->Type() == next);
    }

    bool Parser::Match(TokenType type)
    {
        if (LookAhead(type))
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
        FillLookAhead(1);
        
        return mRead.Dequeue();
    }
    
    void Parser::FillLookAhead(int count)
    {
        while (mRead.Count() < count)
        {
            mRead.Enqueue(mTokens.ReadToken());
        }
    }
}