#include <iostream>

#include "BlockObject.h"
#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ObjectToString)
    {
        return fiber.CreateString(self->AsString());
    }
    
    PRIMITIVE(ObjectSame)
    {
        // Reference equality.
        return fiber.CreateBool(self == args[0]);
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        return self->Parent();
    }
}

