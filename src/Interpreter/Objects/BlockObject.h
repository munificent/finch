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
    
    class CodeBlock;
    
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
            mClosure(closure),
            mCode(NULL)
        {}
        
        ~BlockObject();
        
        const vector<String> &  Params() const { return mParams; }

        //### bob: if we don't want to support metaprogramming (i.e. getting
        // the ast from a block and playing with it at runtime), this can go
        // away
        Ref<Expr>               Body() const { return mBody; }
        
        Ref<Scope>              Closure() const { return mClosure; }
        
        const CodeBlock & GetCode(Environment & environment) const; 
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "block " << mBody;
        }
            
    private:
        vector<String>  mParams;
        Ref<Expr>       mBody;
        Ref<Scope>      mClosure;
        
        // mutable so that it can be lazy initialized in GetCode()
        mutable CodeBlock * mCode;
    };
}