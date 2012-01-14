#pragma once

#include <iostream>

#include "Array.h"
#include "Expr.h"
#include "IExprCompiler.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a block: "{|param| obj message }"
    class BlockExpr : public Expr
    {
    public:
        BlockExpr(const Array<String> & params, Ref<Expr> body)
        :   mParams(params),
            mBody(body)
        {}
        
        const Array<String> & Params() const { return mParams; }
        Ref<Expr>             Body()   const { return mBody; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "{";
            
            if (mParams.Count() > 0)
            {
                stream << "|";
                
                for (int i = 0; i < mParams.Count(); i++)
                {
                    stream << mParams[i];
                    
                    if(i < mParams.Count() - 1) stream << " ";
                }            
                
                stream << "|";
            }
            
            stream << " " << mBody << " }";
        }
            
        EXPRESSION_VISITOR

    private:
        Array<String>  mParams;
        Ref<Expr>      mBody;
    };
}

