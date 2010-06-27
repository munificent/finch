#include "Object.h"
#include "Scope.h"

namespace Finch
{
    void Scope::Define(String name, Ref<Object> value)
    {
        mVariables.Insert(name, value);
    }
    
    void Scope::Set(String name, Ref<Object> value)
    {
        bool found = mVariables.Replace(name, value);
        
        if (!found && !mParent.IsNull())
        {
            // wasn't found, so defer to parent scope
            mParent->Set(name, value);
        }
    }
    
    Ref<Object> Scope::LookUp(String name)
    {
        Ref<Object> variable;
        bool found = mVariables.Find(name, &variable);
        
        // try parent scope (recursively) if we didn't find it here
        if (!found && !mParent.IsNull())
        {
            variable = mParent->LookUp(name);
        }
        
        return variable;
    }
    
    void Scope::Undefine(String name)
    {
        mVariables.Remove(name);
    }
}

