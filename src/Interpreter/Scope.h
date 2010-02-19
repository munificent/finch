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
        
        // Defines a variable at this scope and assigns it the given value.
        // If the variable already exists, it will be overwritten.
        void Define(String name, Ref<Object> value);
        
        // Walks the scope chain to find where the given variable is defined
        // and removes it from that scope. If no variable with that name is
        // found, does nothing.
        void Undefine(String name);
        
        // Walks the scope chain to find where the given variable is defined
        // and assigns it the given value. If no variable with that name is
        // found, does nothing. Returns the variable's previous value.
        Ref<Object> Set(String name, Ref<Object> value);
        
        // Gets the value of the given variable.
        Ref<Object> LookUp(String name);
        
        Ref<Scope> Parent() const { return mParent; }
        
    private:
        Ref<Scope>                 mParent;
        Dictionary<String, Object> mVariables;
    };
}