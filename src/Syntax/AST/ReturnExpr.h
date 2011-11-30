#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a "break" or "return" expression
    class ReturnExpr : public Expr
    {
    public:
        ReturnExpr(bool isReturn, Ref<Expr> result)
        :   mIsReturn(isReturn),
            mResult(result)
        {}
        
        bool IsReturn() const { return mIsReturn; }
        Ref<Expr> Result() const { return mResult; }
        
        virtual void Trace(ostream & stream) const
        {
            if (mIsReturn) {
                stream << "return ";
            } else {
                stream << "break ";
            }
            
            stream << mResult;
        }
        
        EXPRESSION_VISITOR
    
    private:
        bool mIsReturn;
        
        // The result that the unwound block will return.
        Ref<Expr> mResult;
    };
}

