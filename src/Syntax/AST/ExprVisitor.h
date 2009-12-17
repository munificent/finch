#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class BlockExpr;
    class DefExpr;
    class KeywordExpr;
    class NameExpr;
    class NumberExpr;
    class OperatorExpr;
    class SequenceExpr;
    class SetExpr;
    class StringExpr;
    class UnaryExpr;
    class Object;
    
    class ExprVisitor
    {
    public:
        virtual ~ExprVisitor() {}
        
        virtual Ref<Object> Visit(const BlockExpr & expr) = 0;
        virtual Ref<Object> Visit(const DefExpr & expr) = 0;
        virtual Ref<Object> Visit(const KeywordExpr & expr) = 0;
        virtual Ref<Object> Visit(const NameExpr & expr) = 0;
        virtual Ref<Object> Visit(const NumberExpr & expr) = 0;
        virtual Ref<Object> Visit(const OperatorExpr & expr) = 0;
        virtual Ref<Object> Visit(const SequenceExpr & expr) = 0;
        virtual Ref<Object> Visit(const SetExpr & expr) = 0;
        virtual Ref<Object> Visit(const StringExpr & expr) = 0;
        virtual Ref<Object> Visit(const UnaryExpr & expr) = 0;
    };
}