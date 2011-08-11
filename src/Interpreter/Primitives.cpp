#include <iostream>

#include "ArrayObject.h"
#include "ArrayPrimitives.h"
#include "BlockObject.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "FiberObject.h"
#include "Interpreter.h"
#include "IInterpreterHost.h"
#include "Primitives.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    // Built-in prototype accessors.
    
    PRIMITIVE(PrimitiveGetArrayPrototype)
    {
        fiber.Push(fiber.GetEnvironment().ArrayPrototype());
    }

    PRIMITIVE(PrimitiveGetBlockPrototype)
    {
        fiber.Push(fiber.GetEnvironment().BlockPrototype());
    }
    
    PRIMITIVE(PrimitiveGetFiberPrototype)
    {
        fiber.Push(fiber.GetEnvironment().FiberPrototype());
    }

    PRIMITIVE(PrimitiveGetNumberPrototype)
    {
        fiber.Push(fiber.GetEnvironment().NumberPrototype());
    }
    
    PRIMITIVE(PrimitiveGetStringPrototype)
    {
        fiber.Push(fiber.GetEnvironment().StringPrototype());
    }
    
    // Primitive string operators. Note that these get wrapped in the base
    // library so that the arguments can have toString called on them before
    // passing them to the primitives which expect them to already be in string
    // form.
    
    PRIMITIVE(PrimitiveStringConcat)
    {
        fiber.PushString(args[0]->AsString() + args[1]->AsString());
    }
    
    PRIMITIVE(PrimitiveStringCompare)
    {
        fiber.PushNumber(args[0]->AsString().CompareTo(args[1]->AsString()));
    }
    
    PRIMITIVE(PrimitiveWrite)
    {
        String text = args[0]->AsString();
        fiber.GetInterpreter().GetHost().Output(text);
        fiber.PushNil();
    }
    
    // Primitives for manipulating fibers.
    
    PRIMITIVE(PrimitiveNewFiber)
    {
        // get block arg
        BlockObject * block = args[0]->AsBlock();
        if (block == NULL)
        {
            fiber.Error("Must pass in a block object to create a new fiber.");
            fiber.PushNil();
            return;
        }
        
        fiber.Push(Object::NewFiber(fiber.GetInterpreter(), args[0]));
    }
    
    PRIMITIVE(PrimitiveGetCurrentFiber)
    {
        fiber.Push(fiber.GetInterpreter().GetCurrentFiber());
    }
    
    PRIMITIVE(PrimitiveSwitchToFiber)
    {
        // get the fiber we're about switch to
        FiberObject * fiberObj = args[0]->AsFiber();
        
        // if you try to run a completed fiber, it does nothing
        if (fiberObj->GetFiber().IsDone())
        {
            fiber.PushNil();
            return;
        }
        
        // pass the value to the new fiber
        fiberObj->GetFiber().Push(args[1]);
        
        // run the new fiber
        fiber.GetInterpreter().SwitchToFiber(args[0]);
        
        // note that unlike other primitives, we are *not* pushing a result
        // onto the stack of the calling fiber here. when we switch back to
        // this fiber, we'll push the passed value on to the stack then.
    }
    
    PRIMITIVE(PrimitiveGetCallstackDepth)
    {
        fiber.PushNumber(fiber.GetCallstackDepth());
    }
}

