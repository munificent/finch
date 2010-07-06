#include <iostream>

#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ArrayGetPrototype)
    {
        process.Push(process.GetEnvironment().ArrayPrototype());
    }
    
    PRIMITIVE(ArrayLength)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        process.PushNumber(array->Elements().Count());
    }
    
    PRIMITIVE(ArrayAdd)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        array->Elements().Add(args[0]);
        process.Push(thisRef);
    }
    
    PRIMITIVE(ArrayAt)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        // allow negative indexes to index backwards from end
        if ((index >= -array->Elements().Count()) && (index < array->Elements().Count()))
        {
            process.Push(array->Elements()[index]);
        }
        else
        {
            // out of bounds
            process.PushNil();
        }
    }
    
    PRIMITIVE(ArrayAtPut)
    {
        ArrayObject * array = thisRef->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        // allow negative indexes to index backwards from end
        if ((index >= -array->Elements().Count()) && (index < array->Elements().Count()))
        {
            array->Elements()[index] = args[1];
        }

        process.Push(thisRef);
    }
}

