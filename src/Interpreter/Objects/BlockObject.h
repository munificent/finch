#pragma once

#include <iostream>
#include <vector>

#include "CodeBlock.h"
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
        BlockObject(Ref<Object> prototype, const CodeBlock & code, Ref<Scope> closure)
        :   Object(prototype),
            mCode(code),
            mClosure(closure)
        {}
        
        const vector<String> &  Params() const { return mCode.Params(); }

              Ref<Scope>        Closure() const { return mClosure; }
        
        const CodeBlock &       GetCode() const { return mCode; } 
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "block";
        }
            
    private:
        const CodeBlock & mCode;
              Ref<Scope>  mClosure;
    };
}