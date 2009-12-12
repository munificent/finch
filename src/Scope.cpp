#include "Object.h"
#include "Scope.h"

namespace Finch
{
    Ref<Object> Scope::Define(String name, Ref<Object> value)
    {
        map<String, Ref<Object> >::iterator found = mVariables.lower_bound(name);

        //### bob: should probably default to Nil object, not actual null ref
        Ref<Object> oldValue;
        
        if ((found != mVariables.end()) &&
            !(mVariables.key_comp()(name, found->first)))
        {
            // variable already defined, so get the old value then replace it
            oldValue = found->second;
            found->second = value;
        }
        else
        {
            // not defined, so define it now
            mVariables.insert(found,
                map<String, Ref<Object> >::value_type(name, value));
        }

        return oldValue;
    }

    Ref<Object> Scope::Set(String name, Ref<Object> value)
    {
        map<String, Ref<Object> >::iterator found = mVariables.lower_bound(name);
        
        //### bob: should probably default to Nil object, not actual null ref
        Ref<Object> oldValue;
        
        if ((found != mVariables.end()) &&
            !(mVariables.key_comp()(name, found->first)))
        {
            // found it at this scope, so get the old value then replace it
            oldValue = found->second;
            found->second = value;
        }
        else if (!mParent.IsNull())
        {
            // not defined, so defer to parent scope
            oldValue = mParent->Set(name, value);
        }
        
        return oldValue;
    }
    
    Ref<Object> Scope::LookUp(String name)
    {
        map<String, Ref<Object> >::iterator found = mVariables.find(name);
        
        if (found != mVariables.end())
        {
            return found->second;
        }
        
        // not found, try parent scope (recursively)
        if (!mParent.IsNull())
        {
            return mParent->LookUp(name);
        }
        
        // not found
        //### bob: should probably return Nil object, not actual null ref
        return Ref<Object>();
    }
}