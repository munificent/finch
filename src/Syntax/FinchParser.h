#pragma once

#include "Macros.h"
#include "Parser.h"
#include "Expr.h"
#include "Ref.h"

namespace Finch
{
    class ILineReader;
    
    class FinchParser : private Parser
    {
    public:
        FinchParser(ITokenSource * tokens)
        :   Parser(tokens)
        {}
        
        Ref<Expr> Parse();
        Ref<Expr> ParseLine();

    private:
        Ref<Expr> Expression();
        Ref<Expr> Sequence();
        Ref<Expr> Variable();
        Ref<Expr> Keyword();
        Ref<Expr> Operator();
        Ref<Expr> Unary();
        Ref<Expr> Primary();
        
        Ref<Expr> KeywordMessage(Ref<Expr> object);
        
        Ref<Expr> ParseError();
        Ref<Expr> ParseError(const char * message);
        
        NO_COPY(FinchParser)
    };
}