#pragma once

#include <iostream>

#include "Expr.h"
#include "IExprVisitor.h"
#include "Macros.h"
#include "Ref.h"
#include "InternString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for variable eradication: "undef foo"
    class UndefExpr : public Expr
    {
    public:
        UndefExpr(InternString name)
        :   mName(name)
        {}
        
        InternString    Name()  const { return mName; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "undef " << mName;
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        InternString mName;
    };
}