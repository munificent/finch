#pragma once

#include "Macros.h"
#include "Lexer.h"

namespace Finch
{
    class Parser
    {
    public:
        Parser() {}
        
        void StartLine(const char * line);
        
    protected:
        bool            CurrentIs(TokenType type);
        bool            ConsumeIf(TokenType type);
        
        auto_ptr<Token> Consume();
        auto_ptr<Token> Consume(TokenType type);
        
    private:
        Lexer           mLexer;
        auto_ptr<Token> mCurrent;
        
        NO_COPY(Parser)
    };
}