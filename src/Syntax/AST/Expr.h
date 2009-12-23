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
    
    enum NameScope
    {
        NAMESCOPE_GLOBAL,
        NAMESCOPE_OBJECT,
        NAMESCOPE_LOCAL
    };
    
    class Expr
    {
    public:
        // If this Token is a name, this looks at the naming convention to
        // determine what scope a variable with this name will be declared in.
        // The conventions are:
        // - InitialCapial      = global
        // - _leadingUnderscode = object
        // - anythingElse       = local
        static NameScope GetNameScope(String name)
        {
            ASSERT(name.size() > 0, "Cannot determine the scope of an empty name.");
            
            char initial = name[0];
            
            if ((initial >= 'A') && (initial <= 'Z')) return NAMESCOPE_GLOBAL;
            if (initial == '_') return NAMESCOPE_OBJECT;
            return NAMESCOPE_LOCAL;
        }
        
        virtual ~Expr() {}
        
        //### bob: coupling ast to interpreter (object) here is gross.
        // wish you could do template virtual methods. :(
        virtual Ref<Object> Accept(IExprVisitor & visitor) const = 0;
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    ostream & operator<<(ostream & cout, const Expr & expr);
}