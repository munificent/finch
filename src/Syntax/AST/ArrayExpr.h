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
    
    // AST node for an array literal: "[1; 2 + 3; 4 mod: 5]"
    class ArrayExpr : public Expr
    {
    public:
        ArrayExpr(const Array<Ref<Expr> > elements)
        :   mElements(elements)
        {}
        
        const Array<Ref<Expr> > & Elements()  const { return mElements; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "[";
            
            if (mElements.Count() > 0) stream << mElements[0];
            for (int i = 1; i < mElements.Count(); i++)
            {
                stream << "; " << mElements[i];
            }
            stream << "]";
        }
        
        EXPRESSION_VISITOR
        
    private:
        Array<Ref<Expr> > mElements;
    };
}

