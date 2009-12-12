#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    class Expr;
    class Scope;
    
    class Object
    {
    public:
        // virtual constructors
        static Ref<Object> New(Ref<Object> prototype);
        static Ref<Object> New(double value);
        static Ref<Object> New(String value);
        static Ref<Object> New(Ref<Scope> parentScope, Ref<Expr> value);
        
        virtual ~Object() {}
        
        virtual Ref<Object> Receive(Ref<Object> thisRef, String message, vector<Ref<Object> > args) = 0;
        
        virtual double AsNumber() const { return 0; }
        virtual String AsString() const { return ""; }
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    std::ostream& operator<<(std::ostream& cout, const Object & object);
}