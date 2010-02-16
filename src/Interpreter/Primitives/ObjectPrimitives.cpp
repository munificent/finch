#include <iostream>

#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Object.h"

namespace Finch
{
    Ref<Object> ObjectSelf(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        return thisRef;
    }
    
    Ref<Object> ObjectEquals(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        // by default, objects compare using reference equality
        return (thisRef == args[0]) ? env.True() : env.False();
    }
    
    Ref<Object> ObjectNotEquals(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args)
    {
        // by default, objects compare using reference equality
        return (thisRef != args[0]) ? env.True() : env.False();
    }
    
    Ref<Object> ObjectCopy(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        return Object::NewObject(thisRef);
    }
    
    Ref<Object> ObjectCopyWith(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        // create the object
        Ref<Object> copy = Object::NewObject(thisRef);
        
        // run the initialization block
        BlockObject * block = args[0]->AsBlock();
        if (block == NULL)
        {
            env.RuntimeError("copyWith: must be passed a block argument.");
            return Ref<Object>();
        }
        
        vector<Ref<Object> > noArgs;
        env.EvaluateMethod(copy, *block, noArgs);
        
        return copy;
    }
    
    Ref<Object> ObjectAddMethodValue(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        DynamicObject * object = thisRef->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        return object->AddMethod(env, name, value);
    }
}
