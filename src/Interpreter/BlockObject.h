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
        BlockObject(Ref<Scope> parentScope, Ref<Expr> body)
        :   mParentScope(parentScope),
            mBody(body)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual Ref<Object> Receive(String message, vector<Ref<Object> > args);
        
    private:
        Ref<Scope> mParentScope;
        Ref<Expr>  mBody;
    };    
}