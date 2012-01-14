#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "DynamicObject.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ArrayCount)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        return fiber.CreateNumber(array->Elements().Count());
    }
    
    PRIMITIVE(ArrayAdd)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        array->Elements().Add(args[0]);
        return self;
    }
    
    PRIMITIVE(ArrayAt)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        // allow negative indexes to index backwards from end
        if ((index >= -array->Elements().Count()) && (index < array->Elements().Count()))
        {
            return array->Elements()[index];
        }
        else
        {
            // out of bounds
            return fiber.Nil();
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

        return self;
    }
    
    PRIMITIVE(ArrayRemoveAt)
    {
        ArrayObject * array = self->AsArray();
        ASSERT_NOT_NULL(array);
        
        int index = static_cast<int>(args[0]->AsNumber());
        
        // TODO(bob): Bounds check.
        Ref<Object> removed = array->Elements()[index];
        array->Elements().RemoveAt(index);
        return removed;
    }
}

