#include <iostream>

#include "StringPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(StringGetPrototype)
    {
        process.Push(process.GetEnvironment().StringPrototype());
    }
    
    PRIMITIVE(StringAdd)
    {
        //### bob: need to figure out how a primitive can call a non-primitive function
        /*
         // dynamically convert the object to a string
         vector<Ref<Object> > noArgs;
         Ref<Object> toString = args[0]->Receive(args[0], process, "toString", noArgs);
         */
        process.PushString(self->AsString() + args[0]->AsString());
    }
    
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

    PRIMITIVE(StringEquals)
    {
        /*
        // dynamically convert the object to a string
        vector<Ref<Object> > noArgs;
        Ref<Object> toString = args[0]->Receive(args[0], process, "toString", noArgs);
        */
        process.PushBool(self->AsString() == args[0]->AsString());
    }
    
    PRIMITIVE(StringNotEquals)
    {
        /*
        // dynamically convert the object to a string
        vector<Ref<Object> > noArgs;
        Ref<Object> toString = args[0]->Receive(args[0], process, "toString", noArgs);
        */
        process.PushBool(self->AsString() != args[0]->AsString());
    }
    
    PRIMITIVE(StringHashCode)
    {
        process.PushNumber(static_cast<double>(self->AsString().HashCode()));
    }
}

