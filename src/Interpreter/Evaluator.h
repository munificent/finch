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
    class EvalContext;
    
    class Evaluator : private ExprVisitor
    {
    public:
        Evaluator(EvalContext & context)
        :   mContext(context)
        {}
        
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
        virtual Ref<Object> Visit(const StringExpr & expr);
        virtual Ref<Object> Visit(const SymbolExpr & expr);
        virtual Ref<Object> Visit(const UnaryExpr & expr);
        
        Ref<Object> NullToNil(Ref<Object> result) const;
        
        EvalContext & mContext;
    };
}