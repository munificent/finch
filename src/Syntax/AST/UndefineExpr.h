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
    
    // AST node for local variable undefinition: "foo <- undefined"
    class UndefineExpr : public Expr
    {
    public:
        UndefineExpr(String name)
        :   mName(name)
        {}
        
        String    Name()  const { return mName; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mName << " <- undefined";
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        String mName;
    };
}

