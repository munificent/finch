#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    class ArrayExpr;
    class BlockExpr;
    class DefExpr;
    class KeywordExpr;
    class NameExpr;
    class NumberExpr;
    class OperatorExpr;
    class SelfExpr;
    class SequenceExpr;
    class SetExpr;
    class StringExpr;
    class UnaryExpr;
    class UndefineExpr;
    class Object;
    
    // Interface for a class that implements the visitor pattern on expression
    // objects. Allows us to define new operations on expressions without
    // putting the code into the expression classes themselves. Lets us
    // separate out the compiler code from the AST.
    class IExprVisitor
    {
    public:
        virtual ~IExprVisitor() {}
        
        virtual void Visit(const ArrayExpr & expr) = 0;
        virtual void Visit(const BlockExpr & expr) = 0;
        virtual void Visit(const DefExpr & expr) = 0;
        virtual void Visit(const KeywordExpr & expr) = 0;
        virtual void Visit(const NameExpr & expr) = 0;
        virtual void Visit(const NumberExpr & expr) = 0;
        virtual void Visit(const OperatorExpr & expr) = 0;
        virtual void Visit(const SequenceExpr & expr) = 0;
        virtual void Visit(const SelfExpr & expr) = 0;
        virtual void Visit(const SetExpr & expr) = 0;
        virtual void Visit(const StringExpr & expr) = 0;
        virtual void Visit(const UnaryExpr & expr) = 0;
        virtual void Visit(const UndefineExpr & expr) = 0;
    };
}

