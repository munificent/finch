#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "InternString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a string literal: "someSymbol"
    class StringExpr : public Expr
    {
    public:
        StringExpr(InternString value)
        :   mValue(value)
        {}
        
        InternString Value() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << '\"' << mValue << '\"';
        }
            
        EXPRESSION_VISITOR
        
    private:
        InternString mValue;
    };
}