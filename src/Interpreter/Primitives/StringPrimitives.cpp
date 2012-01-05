#include <iostream>

#include "StringPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(StringCount)
    {
        return fiber.CreateNumber(self->AsString().Length());
    }
    
    /*
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
    
    PRIMITIVE(StringFromCount)
    {
        String thisString = self->AsString();
        int    from       = static_cast<int>(args[0]->AsNumber());
        int    count      = static_cast<int>(args[1]->AsNumber());
        
        // TODO(bob): Bounds check.
        String substring = String(thisString.Substring(from, count));
        fiber.PushString(substring);
    }
    
    PRIMITIVE(StringIndexOf)
    {
        String thisString = self->AsString();
        String needle     = args[0]->AsString();
        
        fiber.PushNumber(thisString.IndexOf(needle));
    }
    
    PRIMITIVE(StringHashCode)
    {
        fiber.PushNumber(static_cast<double>(self->AsString().HashCode()));
    }
     */
}

