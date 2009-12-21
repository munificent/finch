#pragma once

#include <iostream>

#include "Dictionary.h"
#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class Object;
    
    // A local variable scope. Basically a dictionary of name -> value pairs.
    class Scope
    {
    public:
        Scope() {}
        Scope(Ref<Scope> parent)
        :   mParent(parent)
        {}
        
        // defines a local variable at this scope and assigns it the given
        // value. returns variable's previous value. if a variable already
        // exists, it will be overwritten.
        Ref<Object> Define(String name, Ref<Object> value);
        
        // walks the scope chain to find where the given variable is defined
        // and assigns it the given value. if no variable with that name is
        // found, does nothing. returns the variable's previous value.
        Ref<Object> Set(String name, Ref<Object> value);
        
        // gets the value of the given variable.
        Ref<Object> LookUp(String name);
        
        Ref<Scope> Parent() const { return mParent; }
        
    private:
        Ref<Scope>                 mParent;
        Dictionary<String, Object> mVariables;
    };
}