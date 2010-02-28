#pragma once

#include <iostream>

#include "Array.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a pair of sequential expressions: "a b . c d"
    class SequenceExpr : public Expr
    {
    public:
        SequenceExpr(const Array<Ref<Expr> > expressions)
        :   mExpressions(expressions)
        {}
        
        const Array<Ref<Expr> > & Expressions()  const { return mExpressions; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mExpressions[0];
            for (int i = 1; i < mExpressions.Count(); i++)
            {
                stream << "; " << mExpressions[i];
            }
        }
            
        EXPRESSION_VISITOR

    private:
        Array<Ref<Expr> > mExpressions;
    };
}