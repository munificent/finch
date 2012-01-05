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
        :   mIdentifier(name),
            mValue(value)
        {}
        
        Identifier & GetIdentifier() { return mIdentifier; }
        Ref<Expr> Value() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mIdentifier.Name() << " <- " << mValue;
        }
            
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        Identifier mIdentifier;
        
        // the initial value
        Ref<Expr> mValue;
    };
}

