#pragma once

#include <iostream>
#include <map>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::map;
    
    class Object;
    
    class Scope
    {
    public:
        // assigns the given value to the given variable. returns the
        // variable's previous value.
        Ref<Object> Assign(String name, Ref<Object> value);
        
        // gets the value of the given variable.
        Ref<Object> LookUp(String name);
        
    private:
        map<String, Ref<Object> > mVariables;
    };
}