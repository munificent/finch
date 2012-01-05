#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Ref.h"
#include "FinchString.h"
#include "Identifier.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for variable assignment: "foo <-- bar"
    class SetExpr : public Expr
    {
    public:
        SetExpr(String name, Ref<Expr> value)
        :   mIdentifier(name),
            mValue(value)
        {}
        
        Identifier & GetIdentifier() { return mIdentifier; }
        Ref<Expr> Value() const { return mValue; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mIdentifier.Name() << " <-- " << mValue;
        }
            
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        Identifier mIdentifier;
        
        // the value
        Ref<Expr> mValue;
    };
}

