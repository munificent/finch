#pragma once

#include "Macros.h"
#include "Lexer.h"
#include "Ref.h"

namespace Finch
{
    class Parser
    {
    public:
        Parser() {}
        
        void StartLine(const char * line);
        
    protected:
        bool       CurrentIs(TokenType type);
        bool       ConsumeIf(TokenType type);
        
        Ref<Token> Consume();
        
    private:
        Lexer      mLexer;
        Ref<Token> mCurrent;
        
        NO_COPY(Parser)
    };
}