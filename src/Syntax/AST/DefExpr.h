#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for variable declaration: "def foo <- bar"
    class DefExpr : public Expr
    {
    public:
        DefExpr(String name, Ref<Expr> value)
        :   mName(name),
            mValue(value)
        {}
        
        String    Name()  const { return mName; }
        
        // May be a null reference if it has no initial value.
        Ref<Expr> Value() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "def " << mName << " <- " << mValue;
        }
            
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        String mName;
        
        // the initial value or a null reference for an uninitialized variable
        Ref<Expr> mValue;
    };
}