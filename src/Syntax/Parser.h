#pragma once

#include "Macros.h"
#include "Lexer.h"
#include "Ref.h"

namespace Finch
{
    class ILineReader;
    class ITokenSource;
    
    class Parser
    {
    public:
        Parser(ITokenSource * tokens)
        :   mTokens(tokens)
        {}

    protected:
        Ref<Token> Current() { return mCurrent; }
        bool       CurrentIs(TokenType type);
        bool       ConsumeIf(TokenType type);
        
        Ref<Token> Consume();

    private:
        ITokenSource * mTokens;
        Ref<Token>     mCurrent;
        
        NO_COPY(Parser)
    };
}