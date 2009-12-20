#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    // AST node for variable assignment: "set foo bar"
    class SetExpr : public Expr
    {
    public:
        SetExpr(String name, Ref<Expr> value)
        :   mName(name),
            mValue(value)
        {}
        
        String    Name()  const { return mName; }
        Ref<Expr> Value() const { return mValue; }
        
        virtual void Trace(std::ostream & stream) const;
        
        EXPRESSION_VISITOR
        
    private:
        // the name of the variable
        String mName;
        
        // the value
        Ref<Expr> mValue;
    };
}