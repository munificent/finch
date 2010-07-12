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
    
    // Primitive flow control. All flow control operations can be implemented
    // in terms of conditionally jumping forward (if) and/or back (while).
    
    PRIMITIVE(PrimitiveIfThenElse)
    {
        // figure out which branch to take
        bool condition = (args[0] == process.GetEnvironment().True());
        Ref<Object> receiver = condition ? args[1] : args[2];
        
        // evaluate the branch
        if (receiver->AsBlock() == NULL)
        {
            // the branch isn't a block, so just push its value directly
            process.Push(receiver);
        }
        else
        {
            // it's a block, so evaluate it
            Array<Ref<Object> > noArgs;
            receiver->Receive(receiver, process, "call", noArgs);
        }
    }
    
    PRIMITIVE(PrimitiveWhileDo)
    {
        process.WhileLoop(args[0], args[1]);
    }
}

