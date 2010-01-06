#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "InternString.h"
#include "Ref.h"

namespace Finch
{
    using std::ostream;
    using std::vector;
    
    // AST node for a block: "{|param| obj message }"
    class BlockExpr : public Expr
    {
    public:
        BlockExpr(vector<InternString> params, Ref<Expr> body)
        :   mParams(params),
            mBody(body)
        {}
        
        vector<InternString>    Params() const { return mParams; }
        Ref<Expr>               Body() const { return mBody; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "{";
            
            if (mParams.size() > 0)
            {
                stream << "|";
                
                for (unsigned int i = 0; i < mParams.size(); i++)
                {
                    stream << mParams[i];
                    
                    if(i < mParams.size() - 1) stream << " ";
                }            
                
                stream << "|";
            }
            
            stream << " " << mBody << " }";
        }
            
        EXPRESSION_VISITOR

    private:
        vector<InternString>    mParams;
        Ref<Expr>               mBody;
    };
}