#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    //### bob: wip
    // Compiles Finch ASTs to bytecode for execution by the interpreter.
    class Compiler : private IExprVisitor2
    {
    public:
        static Ref<CodeBlock> Compile(const Expr & expr);
        
    private:
        Compiler(Ref<CodeBlock> code);
        
        virtual ~Compiler() {}
        
        virtual void Visit(const BlockExpr & expr);
        virtual void Visit(const DefExpr & expr);
        virtual void Visit(const KeywordExpr & expr);
        virtual void Visit(const NameExpr & expr);
        virtual void Visit(const NumberExpr & expr);
        virtual void Visit(const OperatorExpr & expr);
        virtual void Visit(const SequenceExpr & expr);
        virtual void Visit(const SetExpr & expr);
        virtual void Visit(const StringExpr & expr);
        virtual void Visit(const UnaryExpr & expr);
        virtual void Visit(const UndefExpr & expr);
        
        Ref<CodeBlock> mCode;
    };
}