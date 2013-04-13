#pragma once

#include <iostream>

#include "Array.h"
#include "Block.h"
#include "Expr.h"
#include "Macros.h"
#include "IExprCompiler.h"
#include "Object.h"
#include "Ref.h"
#include "Stack.h"

namespace Finch
{
    class DefineExpr;
    
    class Compiler : private IExprCompiler
    {
    public:
        // Compiles the given expression to a new top-level block. Used for
        // compiling REPL expressions.
        static Ref<Block> CompileTopLevel(Interpreter & interpreter, const Expr & expr);
        
    private:
        class Upvalue
        {
        public:
            // Default constructor so we can use this in Arrays.
            Upvalue()
            :   mIsLocal(false),
            mIndex(-1),
            mSlot(-1)
            {}
            
            Upvalue(bool isLocal, int index)
            :   mIsLocal(isLocal),
            mIndex(index),
            mSlot(-1)
            {}
            
            bool IsValid() const { return mIndex != -1; }
            bool IsLocal() const { return mIsLocal; }
            int Index() const { return mIndex; }
            void SetSlot(int slot) { mSlot = slot; }
            int Slot() const { return mSlot; }
            
        private:
            bool mIsLocal;
            int  mIndex;
            int  mSlot;
        };
        
        // Every expression except the last in a sequence discards its result
        // value. This special register number is used to avoid some unnecessary
        // instructions if we know the result will be trashed anyway.
        const static int DISCARD_REGISTER = -1;
        
        Compiler(Interpreter & interpreter, Compiler * parent);
        
        void Compile(int methodId, const Array<String> & params, const Expr & expr);

        virtual ~Compiler() {}

        virtual void Visit(const ArrayExpr & expr, int dest);
        virtual void Visit(const BlockExpr & expr, int dest);
        virtual void Visit(const DefExpr & expr, int dest);
        virtual void Visit(const MessageExpr & expr, int dest);
        virtual void Visit(const NameExpr & expr, int dest);
        virtual void Visit(const NumberExpr & expr, int dest);
        virtual void Visit(const ObjectExpr & expr, int dest);
        virtual void Visit(const ReturnExpr & expr, int dest);
        virtual void Visit(const SequenceExpr & expr, int dest);
        virtual void Visit(const SelfExpr & expr, int dest);
        virtual void Visit(const SetExpr & expr, int dest);
        virtual void Visit(const StringExpr & expr, int dest);
        virtual void Visit(const UndefineExpr & expr, int dest);
        virtual void Visit(const VarExpr & expr, int dest);

        // Load a variable with the given name in the current block.
        void LoadName(const String & name, int dest);

        void ResolveName(Compiler * compiler, const String & name,
            Upvalue * outUpvalue, bool * outIsLocal, int * outIndex,
            Upvalue * outResolvedUpvalue);
        void CompileSetGlobal(const String & name, const Expr & value, int dest);
        void CompileSetField(const String & name, const Expr & value, int dest);
        void CompileNestedBlock(int methodId, const BlockExpr & block, int dest);
        void CompileConstant(const Value & constant, int dest);
        void CompileDefinitions(const DefineExpr & expr, int dest);

        Compiler * GetEnclosingMethod();

        int ReserveRegister();
        void ReleaseRegister(int count = 1);
        
        static int sNextMethodId;
        
        Interpreter & mInterpreter;
        // The compiler for the block containing the block this one is compiling
        // or NULL if this is compiling a top-level block.
        Compiler * mParent;
        Ref<Block> mBlock;
        int mInUseRegisters;
        
        // Names of local variables declared in this block.
        Array<String> mLocals;
        Array<Upvalue> mUpvalues;
        
        // The register containing the nearest enclosing object literal. Within
        // an object literal a reference to 'self' inside a field initializer
        // will refer to the enclosing object and not the current dynamically
        // bound self.
        int mObjectRegister;

        // `true` if this method contains a `return` expression or contains a
        // block that does. Used to disable tail call elimination in methods
        // that have a non-local return since the return needs to be able to
        // find the method on the stack when unwinding.
        bool mHasReturn;

        NO_COPY(Compiler);
    };
}

