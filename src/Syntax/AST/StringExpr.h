#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a string literal: "someSymbol"
    class StringExpr : public Expr
    {
    public:
        StringExpr(String value)
        :   mValue(value)
        {}
        
        String GetValue() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << '\"' << mValue << '\"';
        }
            
        EXPRESSION_VISITOR
        
    private:
        String mValue;
    };
}

