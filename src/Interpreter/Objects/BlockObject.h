#pragma once

#include <iostream>
#include <vector>

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    using std::vector;
    
    // Object class for a block: an invokable expression and the scope that
    // encloses it.
    class BlockObject : public Object
    {
    public:
        BlockObject(Ref<Object> prototype, vector<String> params,
                    Ref<Expr> body, Ref<Scope> closure)
        :   Object(prototype),
            mParams(params),
            mBody(body),
            mClosure(closure)
        {}
        
        const vector<String> &  Params() const { return mParams; }
        //### bob: this should be a CodeBlock. if we want to support metaprogramming,
        // then we'll want to keep both the expr and the compiled codeblock for it.
        Ref<Expr>               Body() const { return mBody; }
        Ref<Scope>              Closure() const { return mClosure; }
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "block " << mBody;
        }
            
    private:
        vector<String>  mParams;
        Ref<Expr>       mBody;
        Ref<Scope>      mClosure;
    };    
}