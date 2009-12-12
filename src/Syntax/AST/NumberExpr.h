#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "String.h"

namespace Finch
{
    // AST node for a number literal: "1234.567"
    class NumberExpr : public Expr
    {
    public:
        NumberExpr(double value)
        :   mValue(value)
        {}
        
        double Value() const { return mValue; }
        
        virtual void Trace(std::ostream & stream) const;
        
        EXPRESSION_VISITOR

    private:
        double mValue;
    };
}