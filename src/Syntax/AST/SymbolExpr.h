#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "String.h"

namespace Finch
{
    // AST node for a symbol literal: "`someSymbol"
    class SymbolExpr : public Expr
    {
    public:
        SymbolExpr(String value)
        :   mValue(value)
        {}
        
        String Value() const { return mValue; }
        
        virtual void Trace(std::ostream & stream) const;
        
        EXPRESSION_VISITOR
        
    private:
        String mValue;
    };
}