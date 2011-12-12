#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ArrayCount)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        fiber.PushNumber(array->Elements().Count());
    }
    
    PRIMITIVE(ArrayAdd)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        array->Elements().Add(args[0]);
        fiber.Push(self);
    }
    
    PRIMITIVE(ArrayAt)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        // allow negative indexes to index backwards from end
        if ((index >= -array->Elements().Count()) && (index < array->Elements().Count()))
        {
            fiber.Push(array->Elements()[index]);
        }
        else
        {
            // out of bounds
            fiber.PushNil();
        }
    }
    
    PRIMITIVE(ArrayAtPut)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        // allow negative indexes to index backwards from end
        if ((index >= -array->Elements().Count()) && (index < array->Elements().Count()))
        {
            array->Elements()[index] = args[1];
        }

        fiber.Push(self);
    }
}

