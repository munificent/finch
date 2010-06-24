#pragma once

#include <iostream>

#include "Dictionary.h"
#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class Object;
    
    // A variable scope. Basically a dictionary of name -> value pairs.
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
        // and assigns it the given value. If no variable with that name is
        // found, does nothing.
        void Set(String name, Ref<Object> value);
        
        // Gets the value of the given variable.
        Ref<Object> LookUp(String name);
        
        // Undefines a variable at this scope.
        // If the variable doesn't exist, it does nothing.
        void Undefine(String name);
        
        // Gets the parent scope of this one, or a null reference if it has
        // none. Looking up will defer to this scope if it couldn't be found in
        // this one. For local variable scopes, the parent will be the enclosing
        // lexical scope. For object variable scopes, the parent will be the
        // parent object.
        Ref<Scope> Parent() const { return mParent; }
        
    private:
        Ref<Scope>                       mParent;
        Dictionary<String, Ref<Object> > mVariables;
    };
}