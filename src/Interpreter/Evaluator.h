#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"

namespace Finch
{
    class Environment;
    
    // The core runtime class for Finch. Given a Finch expression, it evaluates
    // it by walking the tree, performs any side-effects, and returns the
    // result.
    class Evaluator : private IExprVisitor
    {
    public:
        Evaluator(Environment & env)
        :   mEnvironment(env)
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
        virtual Ref<Object> Visit(const UnaryExpr & expr);
        virtual Ref<Object> Visit(const UndefExpr & expr);
        
        Ref<Object> NullToNil(Ref<Object> result) const;
        
        Environment & mEnvironment;
    };
}