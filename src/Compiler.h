#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    //### bob: wip
    /*
    // Compiles Finch ASTs to bytecode for execution by the interpreter.
    class Compiler : private IExprVisitor
    {
    public:
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
    };
    */
}