#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class NameExpr;
    class NumberExpr;
    class UnaryExpr;
    class OperatorExpr;
    class KeywordExpr;
    class SequenceExpr;
    class BlockExpr;
    class Object;
    
    class ExprVisitor
    {
    public:
        virtual ~ExprVisitor() {}
        
        virtual Ref<Object> Visit(const NameExpr & expr) = 0;
        virtual Ref<Object> Visit(const NumberExpr & expr) = 0;
        virtual Ref<Object> Visit(const UnaryExpr & expr) = 0;
        virtual Ref<Object> Visit(const OperatorExpr & expr) = 0;
        virtual Ref<Object> Visit(const KeywordExpr & expr) = 0;
        virtual Ref<Object> Visit(const SequenceExpr & expr) = 0;
        virtual Ref<Object> Visit(const BlockExpr & expr) = 0;
    };
}