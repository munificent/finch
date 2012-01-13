#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a "return" expression.
    class ReturnExpr : public Expr
    {
    public:
        ReturnExpr(Ref<Expr> result)
        :   mResult(result)
        {}
        
        Ref<Expr> Result() const { return mResult; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "return " << mResult;
        }
        
        EXPRESSION_VISITOR
    
    private:
        // The result that the unwound block will return.
        Ref<Expr> mResult;
    };
}

