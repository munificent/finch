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
            interpreter.RuntimeError("copyWith: must be passed a block argument.");
            interpreter.PushNil();
        }
        else
        {
            // push the new object. this is what copyWith: will ultimately
            // return
            interpreter.Push(copy);
            
            // tell the inrepreter to discard the return value of the next
            // called method. this will ditch the value returned by the
            // copyWith: block
            interpreter.DiscardReturn();
            
            // call the copyWith: block as if it were a method on the new object
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
        
        object->AddMethod(interpreter, name, value);
        interpreter.PushNil();
    }
}
