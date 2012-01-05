#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "FinchString.h"
#include "IExprVisitor.h"
#include "Identifier.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a named object: "foo"
    class NameExpr : public Expr
    {
    public:
        NameExpr(String name)
        :   mIdentifier(name)
        {}
        
        Identifier & GetIdentifier() { return mIdentifier; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mIdentifier.Name();
        }
        
        EXPRESSION_VISITOR
        
    private:
        Identifier mIdentifier;
    };
}
