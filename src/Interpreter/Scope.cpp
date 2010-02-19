#include "Object.h"
#include "Scope.h"

namespace Finch
{
    void Scope::Define(String name, Ref<Object> value)
    {
        mVariables.Insert(name, value);
    }
    
    void Scope::Undefine(String name)
    {
        if (!mVariables.Remove(name) && !mParent.IsNull())
        {
            // wasn't found, so defer to parent scope
            mParent->Undefine(name);
        }
    }
    
    Ref<Object> Scope::Set(String name, Ref<Object> value)
    {
        Ref<Object> oldValue = mVariables.Replace(name, value);
        
        if (oldValue.IsNull() && !mParent.IsNull())
        {
            // wasn't found, so defer to parent scope
            oldValue = mParent->Set(name, value);
        }
        
        return oldValue;
    }
    
    Ref<Object> Scope::LookUp(String name)
    {
        Ref<Object> found = mVariables.Find(name);
        
        // try parent scope (recursively) if we didn't find it here
        if (found.IsNull() && !mParent.IsNull())
        {
            found = mParent->LookUp(name);
        }
        
        return found;
    }
}