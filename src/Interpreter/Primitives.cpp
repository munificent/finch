#include <iostream>

#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Interpreter.h"
#include "IInterpreterHost.h"
#include "Primitives.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    // Built-in prototype accessors.
    
    PRIMITIVE(PrimitiveGetArrayPrototype)
    {
        process.Push(process.GetEnvironment().ArrayPrototype());
    }

    PRIMITIVE(PrimitiveGetBlockPrototype)
    {
        process.Push(process.GetEnvironment().BlockPrototype());
    }
    
    PRIMITIVE(PrimitiveGetFiberPrototype)
    {
        process.Push(process.GetEnvironment().FiberPrototype());
    }

    PRIMITIVE(PrimitiveGetNumberPrototype)
    {
        process.Push(process.GetEnvironment().NumberPrototype());
    }
    
    PRIMITIVE(PrimitiveGetObjectPrototype)
    {
        process.Push(process.GetEnvironment().ObjectPrototype());
    }
    
    PRIMITIVE(PrimitiveGetStringPrototype)
    {
        process.Push(process.GetEnvironment().StringPrototype());
    }
    
    // Primitive string operators. Note that these get wrapped in the base
    // library so that the arguments can have toString called on them before
    // passing them to the primitives which expect them to already be in string
    // form.
    
    PRIMITIVE(PrimitiveStringConcat)
    {
        process.PushString(args[0]->AsString() + args[1]->AsString());
    }
    
    PRIMITIVE(PrimitiveStringCompare)
    {
        process.PushNumber(args[0]->AsString().CompareTo(args[1]->AsString()));
    }
    
    PRIMITIVE(PrimitiveWrite)
    {
        String text = args[0]->AsString();
        process.GetInterpreter().GetHost().Output(text);
        process.PushNil();
    }
}

