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
        
    class Expr
    {
    public:
        // Determines if a name is a variable name or a field name. Field names
        // start with a leading underscore.
        static bool IsField(String name)
        {
            ASSERT(name.Length() > 0, "Cannot determine the scope of an empty name.");
            
            return name[0] == '_';
        }
        
        virtual ~Expr() {}
        
        // The visitor pattern.
        virtual void Accept(IExprVisitor & visitor) const = 0;
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    ostream & operator<<(ostream & cout, const Expr & expr);
}

