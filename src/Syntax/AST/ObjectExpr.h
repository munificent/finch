#pragma once

#include <iostream>

#include "Array.h"
#include "DefineExpr.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for an object literal.
    class ObjectExpr : public DefineExpr
    {
    public:
        ObjectExpr(Ref<Expr> parent)
        :   mParent(parent)
        {
        }
        
        Ref<Expr> Parent() const { return mParent; }
        
        virtual void Trace(ostream & stream) const
        {
            // TODO(bob): Implement me.
        }
        
        EXPRESSION_VISITOR
        
    private:
        // The object this one inherits from.
        Ref<Expr> mParent;
    };
}

