#include <iostream>

#include "StringPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(StringLength)
    {
        process.PushNumber(self->AsString().Length());
    }
    
    PRIMITIVE(StringAt)
    {
        String thisString = self->AsString();
        int    index      = static_cast<int>(args[0]->AsNumber());
        
        if ((index >= 0) && (index < thisString.Length()))
        {
            String substring = String(thisString[index]);
            process.PushString(substring);
        }
        else
        {
            // out of bounds
            process.PushNil();
        }
    }

    PRIMITIVE(StringHashCode)
    {
        process.PushNumber(static_cast<double>(self->AsString().HashCode()));
    }
}

