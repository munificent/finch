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
}
