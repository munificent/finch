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
        
        const Token &   Consume();
        const Token &   Consume(TokenType type);
        
    private:
        Lexer           mLexer;
        auto_ptr<Token> mCurrent;
        
        NO_COPY(Parser)
    };
}