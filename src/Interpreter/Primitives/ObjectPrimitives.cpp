#include <iostream>

#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Interpreter.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ObjectSelf)
    {
        interpreter.Push(thisRef);
    }
    
    PRIMITIVE(ObjectEquals)
    {
        // by default, objects compare using reference equality
        interpreter.PushBool(thisRef == args[0]);
    }
    
    PRIMITIVE(ObjectNotEquals)
    {
        // by default, objects compare using reference equality
        interpreter.PushBool(thisRef != args[0]);
    }
    
    PRIMITIVE(ObjectCopy)
    {
        interpreter.Push(Object::NewObject(thisRef));
    }
    
    PRIMITIVE(ObjectCopyWith)
    {
        // create the object
        Ref<Object> copy = Object::NewObject(thisRef);
        
        // run the initialization block
        BlockObject * block = args[0]->AsBlock();
        if (block == NULL)
        {
            interpreter.GetEnvironment().RuntimeError("copyWith: must be passed a block argument.");
            interpreter.PushNil();
        }
        else
        {
            vector<Ref<Object> > noArgs;
            interpreter.CallMethod(copy, *block, noArgs);
        }
    }
    
    PRIMITIVE(ObjectAddMethodValue)
    {
        DynamicObject * object = thisRef->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        object->AddMethod(interpreter.GetEnvironment(), name, value);
        interpreter.PushNil();
    }
}
