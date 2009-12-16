#include "DynamicObject.h"
#include "BlockObject.h"
#include "EvalContext.h"

namespace Finch
{
    void DynamicObject::Trace(std::ostream & stream) const
    {
        stream << mName;
    }
    
    Ref<Object> DynamicObject::Receive(Ref<Object> thisRef, EvalContext & context, 
                                       String message, const vector<Ref<Object> > & args)
    {
        if (message == "copy")
        {
            return Object::New(thisRef);
        }
        
        if (message == "addField:value:")
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
        
        if (message == "addMethod:body:")
        {
            String      name  = args[0]->AsString();
            Ref<Object> value = args[1];
            
            //### bob: need better error handling
            if (name.size() == 0)
            {
                std::cout << "oops. need a name to add a method." << std::endl;
                return Ref<Object>();
            }
            
            if (value->AsBlock() == NULL)
            {
                std::cout << "oops. 'body:' argument must be a block." << std::endl;
                return Ref<Object>();
            }
            
            //### bob: should this fail if the method already exists?
            
            // add the method
            mMethods[name] = value;
            
            return Ref<Object>();
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
        
        // see if it's a method call
        found = mMethods.find(message);
        if (found != mMethods.end())
        {
            BlockObject* block = found->second->AsBlock();
            
            ASSERT_NOT_NULL(block);
            
            Ref<Object> result = context.EvaluateMethod(thisRef, block->Body());
            
            return result;
        }
        
        // see if it's a primitive call
        map<String, PrimitiveMethod>::iterator primitive = mPrimitives.find(message);
        if (primitive != mPrimitives.end())
        {
            PrimitiveMethod method = primitive->second;
            
            ASSERT_NOT_NULL(method);
            
            return method(thisRef, context, message, args);
        }
        
        return Object::Receive(thisRef, context, message, args);
    }
    
    void DynamicObject::RegisterPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives[message] = method;
    }
}