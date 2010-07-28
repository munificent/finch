#pragma once

#include <cctype>
#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

#define EXPRESSION_VISITOR                                       \
        virtual void Accept(IExprVisitor & visitor) const        \
        {                                                        \
            visitor.Visit(*this);                                \
        }

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
        // - InitialCapital     = global
        // - _leadingUnderscode = object
        // - anythingElse       = local
        static NameScope GetNameScope(String name)
        {
            ASSERT(name.Length() > 0, "Cannot determine the scope of an empty name.");
            
            // find the first non-punctuation character
            for (int i = 0; i < name.Length(); i++)
            {
                char initial = name[i];
                
                if ((initial >= 'a') && (initial <= 'z')) return NAMESCOPE_GLOBAL;
                if ((initial >= 'A') && (initial <= 'Z')) return NAMESCOPE_GLOBAL;
                if (initial == '_') return NAMESCOPE_OBJECT;
            }
            
            ASSERT(false, "Cannot determine the scope of a name with no letters.");
            return NAMESCOPE_LOCAL;
        }
        
        virtual ~Expr() {}
        
        // The visitor pattern.
        virtual void Accept(IExprVisitor & visitor) const = 0;
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    ostream & operator<<(ostream & cout, const Expr & expr);
}

