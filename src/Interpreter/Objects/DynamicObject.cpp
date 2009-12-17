#include "DynamicObject.h"
#include "BlockObject.h"
#include "Environment.h"

namespace Finch
{
    void DynamicObject::Trace(std::ostream & stream) const
    {
        stream << mName;
    }
    
    Ref<Object> DynamicObject::Receive(Ref<Object> thisRef, Environment & env, 
                                       String message, const vector<Ref<Object> > & args)
    {
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
        
        // see if it's a method call
        found = mMethods.find(message);
        if (found != mMethods.end())
        {
            BlockObject* block = found->second->AsBlock();
            
            ASSERT_NOT_NULL(block);
            
            Ref<Object> result = env.EvaluateMethod(thisRef, block->Body());
            
            return result;
        }
        
        // see if it's a primitive call
        map<String, PrimitiveMethod>::iterator primitive = mPrimitives.find(message);
        if (primitive != mPrimitives.end())
        {
            PrimitiveMethod method = primitive->second;
            
            ASSERT_NOT_NULL(method);
            
            return method(thisRef, env, message, args);
        }
        
        return Object::Receive(thisRef, env, message, args);
    }
    
    Ref<Object> DynamicObject::AddField(String name, Ref<Object> value)
    {
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
    
    Ref<Object> DynamicObject::AddMethod(String name, Ref<Object> body)
    {
        //### bob: need better error handling
        if (name.size() == 0)
        {
            std::cout << "oops. need a name to add a method." << std::endl;
            return Ref<Object>();
        }
        
        if (body->AsBlock() == NULL)
        {
            std::cout << "oops. 'body:' argument must be a block." << std::endl;
            return Ref<Object>();
        }
        
        //### bob: should this fail if the method already exists?
        
        // add the method
        mMethods[name] = body;
        
        return Ref<Object>();        
    }
    
    void DynamicObject::RegisterPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives[message] = method;
    }
}