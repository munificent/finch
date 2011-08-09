#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for variable definition: "foo <- bar"
    class VarExpr : public Expr
    {
    public:
        VarExpr(String name, Ref<Expr> value)
        :   mName(name),
            mValue(value)
        {}
        
        String    Name()  const { return mName; }
        Ref<Expr> Value() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mName << " <- " << mValue;
        }
            
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        String mName;
        
        // the initial value
        Ref<Expr> mValue;
    };
}

