#pragma once

#include "Macros.h"
#include "Lexer.h"
#include "ITokenReader.h"

namespace Finch
{
    class Parser : private ITokenReader
    {
    public:
        Parser()
        :   mLexer(this)
        {}
        
        void Process(const char * line);
        
    private:
        virtual void Read(Token token);
        
        Lexer mLexer;
        
        NO_COPY(Parser)
    };
}