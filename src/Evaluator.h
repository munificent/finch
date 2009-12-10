#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    class Evaluator : private ExprVisitor
    {
    public:
        void Evaluate(Ref<Expr> expr);
        
    private:
        virtual Ref<Object> Visit(const NameExpr & expr);
        virtual Ref<Object> Visit(const NumberExpr & expr);
        virtual Ref<Object> Visit(const UnaryExpr & expr);
        virtual Ref<Object> Visit(const OperatorExpr & expr);
        virtual Ref<Object> Visit(const KeywordExpr & expr);
        virtual Ref<Object> Visit(const SequenceExpr & expr);
        virtual Ref<Object> Visit(const BlockExpr & expr);
    };
}