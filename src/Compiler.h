#pragma once

#include <iostream>

#include "CodeBlock.h"
#include "Expr.h"
#include "Macros.h"
#include "IExprVisitor.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    class Environment;
    
    //### bob: wip
    // Compiles Finch ASTs to bytecode for execution by the interpreter.
    class Compiler : private IExprVisitor2
    {
    public:
        static Ref<CodeBlock> Compile(Environment & environment, const Expr & expr);
        
    private:
        Compiler(Environment & environment, Ref<CodeBlock> code);
        
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
        
        Environment &  mEnvironment;
        Ref<CodeBlock> mCode;
        
        NO_COPY(Compiler);
    };
}