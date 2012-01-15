#include <iostream>

#include "BlockObject.h"
#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ObjectToString)
    {
        return fiber.CreateString(self.AsString());
    }
    
    PRIMITIVE(ObjectSame)
    {
        // Reference equality.
        return fiber.CreateBool(self.Obj() == args[0].Obj());
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        return self.Obj()->Parent();
    }
}

