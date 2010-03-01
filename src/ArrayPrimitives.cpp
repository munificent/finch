#include <iostream>

#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Interpreter.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ArrayLength)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        interpreter.PushNumber(array->Elements().Count());
    }
    
    PRIMITIVE(ArrayAdd)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        array->Elements().Add(args[0]);
        interpreter.PushNil();
    }
    
    PRIMITIVE(ArrayAt)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        if ((index >= 0) && (index < array->Elements().Count()))
        {
            interpreter.Push(array->Elements()[index]);
        }
        else
        {
            // out of bounds
            interpreter.PushNil();
        }
    }
    
    PRIMITIVE(ArrayAtPut)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        if ((index >= 0) && (index < array->Elements().Count()))
        {
            array->Elements()[index] = args[1];
        }

        interpreter.PushNil();
    }
    
    PRIMITIVE(ArrayNew)
    {
        interpreter.Push(Object::NewArray(interpreter.GetEnvironment()));
    }
    
    //### bob: this could be moved into base instead of being a primitive
    PRIMITIVE(ArrayNewWith)
    {
        // create the array
        Ref<Object> object = Object::NewArray(interpreter.GetEnvironment());
        ArrayObject * array = object->AsArray();

        // fill the elements
        int count = static_cast<int>(args[0]->AsNumber());
        for (int i = 0; i < count; i++)
        {
            array->Elements().Add(args[1]);
        }
        
        // push it
        interpreter.Push(object);
    }
}
