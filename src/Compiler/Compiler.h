#pragma once

#include <iostream>

#include "Array.h"
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
    // Each instance of this is used to compile a single block. It keeps track
    // of the parent compiler which is its lexically-enclosing block.
    class Compiler : private IExprVisitor
    {
    public:
        // Compiles the given expression in the given Environment and writes
        // the bytecode into the given CodeBlock. Needs an Environment to have
        // access to the string and code tables.
        static Ref<CodeBlock> CompileTopLevel(Environment & environment,
                                              const Expr & expr);

    private:
        Compiler(Environment & environment, Compiler * parent);

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

        // Compiles the given block with this compiler.
        void Compile(const Array<String> & params, const Expr & body, int methodId);
        
        // Compiles the given block or method contained within this compiler's
        // lexical scope.
        int CompileBlock(const BlockExpr & expr, int methodId);
        void CompileDefinitions(const DefineExpr & expr);

        Compiler * GetEnclosingMethod();
        int GetEnclosingMethodId();
        void SetHasReturn();
        
        static int sNextMethodId;

        Compiler * mParent;
        Environment & mEnvironment;
        Ref<CodeBlock> mCode;
        bool mHasReturn;
        
        NO_COPY(Compiler);
    };
}

