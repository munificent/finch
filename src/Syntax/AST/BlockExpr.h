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
    
    // AST node for a block: "{|param| obj message }"
    class BlockExpr : public Expr
    {
    public:
        BlockExpr(vector<String> params, Ref<Expr> body)
        :   mParams(params),
            mBody(body)
        {}
        
        vector<String>  Params() const { return mParams; }
        Ref<Expr>       Body() const { return mBody; }
        
        virtual void Trace(std::ostream & stream) const;
        
        EXPRESSION_VISITOR

    private:
        vector<String> mParams;
        Ref<Expr>      mBody;
    };
}