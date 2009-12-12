#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"

namespace Finch
{
    class Evaluator : private ExprVisitor
    {
    public:
        Evaluator();
        Evaluator(Ref<Scope> parentScope);
        
        Ref<Object> Evaluate(Ref<Expr> expr);
        
    private:
        virtual Ref<Object> Visit(const BlockExpr & expr);
        virtual Ref<Object> Visit(const DefExpr & expr);
        virtual Ref<Object> Visit(const KeywordExpr & expr);
        virtual Ref<Object> Visit(const NameExpr & expr);
        virtual Ref<Object> Visit(const NumberExpr & expr);
        virtual Ref<Object> Visit(const OperatorExpr & expr);
        virtual Ref<Object> Visit(const SequenceExpr & expr);
        virtual Ref<Object> Visit(const SetExpr & expr);
        virtual Ref<Object> Visit(const UnaryExpr & expr);
        
        Ref<Scope> mScope;
    };
}