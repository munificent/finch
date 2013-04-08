#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

// TODO(bob): Rename file.
namespace Finch
{
    class ArrayExpr;
    class BlockExpr;
    class DefExpr;
    class MessageExpr;
    class NameExpr;
    class NumberExpr;
    class ObjectExpr;
    class ReturnExpr;
    class SelfExpr;
    class SequenceExpr;
    class SetExpr;
    class StringExpr;
    class UndefineExpr;
    class VarExpr;
    class Object;
    
    // Visitor pattern used by the compiler to compile AST to bytecode.
    class IExprCompiler
    {
    public:
        virtual ~IExprCompiler() {}
        
        virtual void Visit(const ArrayExpr & expr, int dest) = 0;
        virtual void Visit(const BlockExpr & expr, int dest) = 0;
        virtual void Visit(const DefExpr & expr, int dest) = 0;
        virtual void Visit(const MessageExpr & expr, int dest) = 0;
        virtual void Visit(const NameExpr & expr, int dest) = 0;
        virtual void Visit(const NumberExpr & expr, int dest) = 0;
        virtual void Visit(const ObjectExpr & expr, int dest) = 0;
        virtual void Visit(const ReturnExpr & expr, int dest) = 0;
        virtual void Visit(const SequenceExpr & expr, int dest) = 0;
        virtual void Visit(const SelfExpr & expr, int dest) = 0;
        virtual void Visit(const SetExpr & expr, int dest) = 0;
        virtual void Visit(const StringExpr & expr, int dest) = 0;
        virtual void Visit(const UndefineExpr & expr, int dest) = 0;
        virtual void Visit(const VarExpr & expr, int dest) = 0;
    };
}

