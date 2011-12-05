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
        fiber.PushString(self->AsString());
    }
    
    PRIMITIVE(ObjectSame)
    {
        // reference equality
        fiber.PushBool(self == args[0]);
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        fiber.Push(self->GetParent());
    }
    
    PRIMITIVE(ObjectSetParent)
    {
        self->SetParent(args[0]);
        fiber.PushNil();
    }
}

