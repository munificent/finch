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
        return fiber.CreateBool(self == args[0]);
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        Value parent = self.Parent();
        
        // If we don't have a parent, we're at Object, so just return Object
        // as its own parent.
        if (parent.IsNull()) return self;
        
        return parent;
    }
}

