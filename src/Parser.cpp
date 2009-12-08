#include <iostream>
#include <memory>

#include "Parser.h"

namespace Finch
{
    using std::auto_ptr;
    
    void Parser::StartLine(const char * line)
    {
        mLexer.StartLine(line);
        mCurrent.reset();
    }

    bool Parser::CurrentIs(TokenType type)
    {
        if (mCurrent.get() == NULL) mCurrent = mLexer.ReadToken();
        
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
    
    auto_ptr<Token> Parser::Consume()
    {
        if (mCurrent.get() == NULL) mCurrent = mLexer.ReadToken();
        
        // return the current token. note that this will clear the auto_ptr
        // so that we'll know to lex the next token later
        return mCurrent;
    }
    
    auto_ptr<Token> Parser::Consume(TokenType type)
    {
        //### bob: need to check that current is right type!
        return Consume();
    }
}