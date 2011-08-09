#pragma once

#include "Array.h"
#include "Macros.h"
#include "Parser.h"
#include "Expr.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class IErrorReporter;
    class ILineReader;
    class MessageExpr;
    
    // Parser for the Finch grammar.
    class FinchParser : public Parser
    {
    public:
        FinchParser(ITokenSource & tokens, IErrorReporter & errorReporter)
        :   Parser(tokens),
            mErrorReporter(errorReporter)
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
        Ref<Expr> Cascade();
        Ref<Expr> Keyword(bool & isMessage);
        Ref<Expr> Operator(bool & isMessage);
        Ref<Expr> Unary(bool & isMessage);
        Ref<Expr> Primary();
        
        Ref<Expr> ParseSequence(Array<Ref<Expr> > & expressions);
        Ref<Expr> ParseKeyword(Ref<Expr> object);        
        Ref<Expr> ParseError(const char * message);
        
        Ref<Expr> ParseBind(Ref<Expr> expr);
        Ref<Expr> ParseBindBody(Ref<Expr> expr, String name,
                           const Array<String> & args);

        IErrorReporter & mErrorReporter;
        
        NO_COPY(FinchParser);
    };
}

