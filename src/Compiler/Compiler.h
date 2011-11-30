#pragma once

#include <iostream>

#include "CodeBlock.h"
#include "Expr.h"
#include "Macros.h"
#include "IExprVisitor.h"
#include "Object.h"
#include "Ref.h"
#include "Stack.h"

namespace Finch
{
    class DefineExpr;
    class Environment;
    
    // Compiles an expression AST to bytecode for execution by the interpreter.
    class Compiler : private IExprVisitor
    {
    public:
        // Compiles the given expression in the given Environment and writes
        // the bytecode into the given CodeBlock. Needs an Environment to have
        // access to the string and code tables.
        static void Compile(Environment & environment, const Expr & expr,
                            CodeBlock & code, int enclosingMethod);
        
    private:
        Compiler(Environment & environment, CodeBlock & code,
                 int enclosingMethod);
        
        virtual ~Compiler() {}
        
        virtual void Visit(const ArrayExpr & expr);
        virtual void Visit(const BindExpr & expr);
        virtual void Visit(const BlockExpr & expr);
        virtual void Visit(const MessageExpr & expr);
        virtual void Visit(const NameExpr & expr);
        virtual void Visit(const NumberExpr & expr);
        virtual void Visit(const ObjectExpr & expr);
        virtual void Visit(const ReturnExpr & expr);
        virtual void Visit(const SequenceExpr & expr);
        virtual void Visit(const SelfExpr & expr);
        virtual void Visit(const SetExpr & expr);
        virtual void Visit(const StringExpr & expr);
        virtual void Visit(const UndefineExpr & expr);
        virtual void Visit(const VarExpr & expr);
        
        void CompileDefinitions(const DefineExpr & expr);
        
        static int sNextMethodId;
        
        Environment & mEnvironment;
        CodeBlock & mCode;
        
        int mEnclosingMethod;
        int mMethodIdForNextBlock;

        NO_COPY(Compiler);
    };
}

