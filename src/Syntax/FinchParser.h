#pragma once

#include "Array.h"
#include "Expr.h"
#include "FinchString.h"
#include "Macros.h"
#include "Parser.h"
#include "Ref.h"

namespace Finch
{
    class ILineReader;
    class MessageExpr;
        
    // Parser for the Finch grammar.
    class FinchParser : public Parser
    {
    public:
        FinchParser(ITokenSource & tokens, IErrorReporter & errorReporter)
        :   Parser(tokens, errorReporter)
        {}
        
        virtual ~FinchParser() {}
        
        // Reads from the token source and returns the parsed expression. If
        // this is an infinite source, it will return as soon as a complete
        // expression is parsed. Otherwise, it will parse the entire source.
        Ref<Expr> Parse();

    private:
        // The grammar productions, from lowest to highest precedence.
        Ref<Expr> Expression();
        Ref<Expr> Sequence();
        Ref<Expr> Variable();
        Ref<Expr> Bind();
        Ref<Expr> Assignment();
        Ref<Expr> Keyword();
        Ref<Expr> Operator();
        Ref<Expr> Unary();
        Ref<Expr> Primary();

        // Parses just the message send part of a message "foo(1, 2) bar(3)"
        Ref<Expr> ParseMessage(Ref<Expr> receiver);

        // Parses just the message send part of a keyword message: "foo: a bar: b"
        Ref<Expr> ParseKeyword(Ref<Expr> object);

        // Parses a block literal. Assumes the "{" has already been consumed.
        Ref<Expr> ParseBlock();

        void ParseDefines(DefineExpr & expr, TokenType endToken);
        void ParseDefine(DefineExpr & expr);
        void ParseDefineBody(DefineExpr & expr, String name,
            const Array<String> & params);
        
        NO_COPY(FinchParser);
    };
}

