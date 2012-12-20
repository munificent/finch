#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprCompiler.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for the "self" reserved word
    class SelfExpr : public Expr
    {
    public:
        SelfExpr()
        {}
        
        virtual void Trace(ostream & stream) const
        {
            stream << "self";
        }
        
        EXPRESSION_VISITOR
    };
}

