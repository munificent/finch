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
        BlockObject(Ref<Expr> body)
        :   mBody(body)
        {}
        
        Ref<Expr> Body() const { return mBody; }
        
        virtual Ref<Object> Receive(Ref<Object> thisRef, EvalContext & context,
                                    String message, const vector<Ref<Object> > & args);
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(std::ostream & stream) const;
        
    private:
        Ref<Expr>  mBody;
    };    
}