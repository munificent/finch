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
    
    PRIMITIVE(ObjectEquals)
    {
        // by default, objects compare using reference equality
        fiber.PushBool(self == args[0]);
    }
        
    PRIMITIVE(ObjectAddMethodBody)
    {
        DynamicObject * object = self->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        object->AddMethod(self, fiber, name, value);
        fiber.PushNil();
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

