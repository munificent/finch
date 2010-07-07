#pragma once

#include "Array.h"
#include "Macros.h"
#include "Parser.h"
#include "Expr.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class ILineReader;
    
    // Parser for the Finch grammar.
    class FinchParser : public Parser
    {
    public:
        FinchParser(ITokenSource & tokens)
        :   Parser(tokens)
        {}
        
        // Reads from the token source and returns the parsed expression. If
        // this is an infinite source, it will return as soon as a complete
        // expression is parsed. Otherwise, it will parse the entire source.
        Ref<Expr> Parse();

    private:
        // The grammar productions, from lowest to highest precedence.
        Ref<Expr> Expression();
        Ref<Expr> Sequence();
        Ref<Expr> Bind();
        Ref<Expr> Assignment();
        Ref<Expr> Keyword();
        Ref<Expr> Operator();
        Ref<Expr> Unary();
        Ref<Expr> Primary();
        
        void ParseSequence(Array<Ref<Expr> > & expressions);
        Ref<Expr> ParseKeyword(Ref<Expr> object);
        Ref<Expr> ParseBindBody(Ref<Expr> target, String name,
                                const Array<String> & args);
        
        Ref<Expr> ParseError();
        Ref<Expr> ParseError(const char * message);
        
        NO_COPY(FinchParser);
    };
}

