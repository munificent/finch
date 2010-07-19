#include <iostream>

#include "StringPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(StringLength)
    {
        fiber.PushNumber(self->AsString().Length());
    }
    
    PRIMITIVE(StringAt)
    {
        String thisString = self->AsString();
        int    index      = static_cast<int>(args[0]->AsNumber());
        
        if ((index >= 0) && (index < thisString.Length()))
        {
            String substring = String(thisString[index]);
            fiber.PushString(substring);
        }
        else
        {
            // out of bounds
            fiber.PushNil();
        }
    }

    PRIMITIVE(StringHashCode)
    {
        fiber.PushNumber(static_cast<double>(self->AsString().HashCode()));
    }
}

