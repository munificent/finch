#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Expr.h"
#include "ExprVisitor.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    // AST node for a block: "{|arg| obj message }"
    class BlockExpr : public Expr
    {
    public:
        BlockExpr(vector<String> args, Ref<Expr> body)
        :   mArgs(args),
            mBody(body)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        EXPRESSION_VISITOR

    private:
        vector<String> mArgs;
        Ref<Expr>      mBody;
    };
}