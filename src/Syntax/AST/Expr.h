#pragma once

#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

#define EXPRESSION_VISITOR                                       \
        virtual Ref<Object> Accept(IExprVisitor & visitor) const \
        {                                                        \
            return visitor.Visit(*this);                         \
        }                                                        \

namespace Finch
{
    using std::ostream;
    
    class IExprVisitor;
    class Object;
    
    class Expr
    {
    public:
        virtual ~Expr() {}
        
        //### bob: coupling ast to interpreter (object) here is gross.
        // wish you could do template virtual methods. :(
        virtual Ref<Object> Accept(IExprVisitor & visitor) const = 0;
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    ostream & operator<<(ostream & cout, const Expr & expr);
}