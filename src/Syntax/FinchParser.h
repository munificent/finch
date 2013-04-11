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
        Ref<Expr> Statement();
        Ref<Expr> Assignment();
        Ref<Expr> Operator();
        Ref<Expr> Message();
        Ref<Expr> Primary();

        // Parses just the message send part of a message "foo(1, 2) bar(3)"
        Ref<Expr> ParseMessage(Ref<Expr> receiver);

        // Parses a block literal. Assumes the "{" has already been consumed.
        Ref<Expr> ParseBlock();

        void ParseDefines(DefineExpr & expr, TokenType endToken);
        void ParseDefine(DefineExpr & expr);
        void ParseDefineBody(DefineExpr & expr, String name,
            const Array<String> & params);
        
        NO_COPY(FinchParser);
    };

    // Walks an AST, replacing "_" names with uniquely generated names.
    class ImplicitArgumentTranslator : public IExprVisitor
    {
    public:
        ImplicitArgumentTranslator(Array<String> & params)
        :   mParams(params)
        {}
        
        virtual void Visit(ArrayExpr & expr);
        virtual void Visit(BlockExpr & expr);
        virtual void Visit(DefExpr & expr);
        virtual void Visit(MessageExpr & expr);
        virtual void Visit(NameExpr & expr);
        virtual void Visit(NumberExpr & expr);
        virtual void Visit(ObjectExpr & expr);
        virtual void Visit(ReturnExpr & expr);
        virtual void Visit(SequenceExpr & expr);
        virtual void Visit(SelfExpr & expr);
        virtual void Visit(SetExpr & expr);
        virtual void Visit(StringExpr & expr);
        virtual void Visit(UndefineExpr & expr);
        virtual void Visit(VarExpr & expr);

    private:
        Array<String> & mParams;
    };
}

