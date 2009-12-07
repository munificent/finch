#pragma once

#include "Macros.h"
#include "Parser.h"
#include "Expr.h"

namespace Finch
{
    class FinchParser : private Parser
    {
    public:
        FinchParser() {}
        
        auto_ptr<Expr> ParseLine(const char * line);
        
    private:
        auto_ptr<Expr> Expression();
        auto_ptr<Expr> Keyword();
        auto_ptr<Expr> Operator();
        auto_ptr<Expr> Unary();
        auto_ptr<Expr> Primary();
        
        auto_ptr<Expr> ParseError();
        
        NO_COPY(FinchParser)
    };
}