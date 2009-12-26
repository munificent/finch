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
    
    // AST node for variable eradication: "undef foo"
    class UndefExpr : public Expr
    {
    public:
        UndefExpr(String name)
        :   mName(name)
        {}
        
        String    Name()  const { return mName; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "undef " << mName;
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        String mName;
    };
}