#include <iostream>

#include "BlockObject.h"
#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(ObjectToString)
    {
        process.PushString(self->AsString());
    }
    
    PRIMITIVE(ObjectEquals)
    {
        // by default, objects compare using reference equality
        process.PushBool(self == args[0]);
    }
        
    PRIMITIVE(ObjectAddMethodBody)
    {
        DynamicObject * object = self->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        object->AddMethod(self, process, name, value);
        process.PushNil();
    }
    
    PRIMITIVE(ObjectGetParent)
    {
        process.Push(self->GetParent());
    }
    
    PRIMITIVE(ObjectSetParent)
    {
        self->SetParent(args[0]);
        process.PushNil();
    }
}

