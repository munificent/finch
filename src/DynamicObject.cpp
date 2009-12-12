#include "DynamicObject.h"

namespace Finch
{
    void DynamicObject::Trace(std::ostream & stream) const
    {
        stream << "object";
    }
    
    Ref<Object> DynamicObject::Receive(String message, vector<Ref<Object> > args)
    {
        if (message == "copy")
        {
            //### bob: temp. just make a new empty object. should actually
            // make a new one with its prototype set to this
            return Object::New();
        }
        
        if (message == "add-field:value:")
        {
            String      name  = args[0]->AsString();
            Ref<Object> value = args[1];
            
            if (name.size() == 0)
            {
                //### bob: need better error handling
                std::cout << "oops. need a name to add a field." << std::endl;
                return Ref<Object>();
            }
            
            //### bob: should this fail if the field already exists?
            
            // add the field
            mFields[name] = value;
            
            return value;
        }
        
        // see if it's a field access
        map<String, Ref<Object> >::iterator found = mFields.find(message);
        if (found != mFields.end())
        {
            return found->second;
        }
        
        // see if it's a field assignment
        if ((args.size() == 1) && (message[message.size() - 1] == ':'))
        {
            String fieldAssign = message.substr(0, message.size() - 1);

            //### bob: copy/pasted from Scope.cpp. need Dict class
            found = mFields.lower_bound(fieldAssign);
            
            Ref<Object> oldValue;
            
            if ((found != mFields.end()) &&
                !(mFields.key_comp()(fieldAssign, found->first)))
            {
                // found it at this scope, so get the old value then replace it
                oldValue = found->second;
                found->second = args[0];
                
                return oldValue;
            }
        }
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}