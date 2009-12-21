#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a number literal: "1234.567"
    class NumberExpr : public Expr
    {
    public:
        NumberExpr(double value)
        :   mValue(value)
        {}
        
        double Value() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mValue;
        }
        
        EXPRESSION_VISITOR

    private:
        double mValue;
    };
}