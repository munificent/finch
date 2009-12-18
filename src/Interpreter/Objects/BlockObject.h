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
    using std::vector;
    
    class BlockObject : public Object
    {
    public:
        BlockObject(Ref<Object> prototype, vector<String> params, Ref<Expr> body)
        :   Object(prototype),
            mParams(params),
            mBody(body)
        {}
        
        const vector<String> &  Params() const { return mParams; }
        Ref<Expr>               Body() const { return mBody; }
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(std::ostream & stream) const;
        
    private:
        vector<String>  mParams;
        Ref<Expr>       mBody;
    };    
}