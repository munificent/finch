#pragma once

#include "Macros.h"
#include "Parser.h"
#include "Expr.h"
#include "Ref.h"

namespace Finch
{
    class FinchParser : private Parser
    {
    public:
        FinchParser() {}
        
        Ref<Expr> ParseLine(const char * line);
        
    private:
        Ref<Expr> Expression();
        Ref<Expr> Sequence();
        Ref<Expr> Keyword();
        Ref<Expr> Operator();
        Ref<Expr> Unary();
        Ref<Expr> Primary();
        
        Ref<Expr> ParseError();
        
        NO_COPY(FinchParser)
    };
}