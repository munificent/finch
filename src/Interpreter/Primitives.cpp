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
    
    // Primitive object operations.
    
    PRIMITIVE(PrimitiveCopy)
    {
        process.Push(Object::NewObject(args[0]));
    }
    
    PRIMITIVE(PrimitiveRunWithin)
    {
        // make sure we have a block
        BlockObject * originalBlock = args[0]->AsBlock();
        if (originalBlock == NULL)
        {
            process.Error("run:within: must be passed a block argument.");
            process.PushNil();
            return;
        }
        
        //### bob: update docs
        // bind the block's "self" to the given target.
        Ref<Object> blockCopy = Object::NewBlock(process.GetEnvironment(),
                                                 originalBlock->GetCode(),originalBlock->Closure(),
                                                 args[1]);
        
        // we copy the block here because, in theory, you could reuse the block after passing it to
        // copyWith:, like:
        //
        // b <- { writeLine: "self is " + self }
        // c <- Object copyWith: b
        // b call
        // d <- Object copyWith: b
        // b call
        // since binding self mutates it, we have to copy it first
        
        // now call the block
        process.CallMethod(args[1], blockCopy, Array<Ref<Object> >());
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
    
    // Primitives for manipulating fibers.
    
    PRIMITIVE(PrimitiveNewFiber)
    {
        // get block arg
        BlockObject * block = args[0]->AsBlock();
        if (block == NULL)
        {
            process.Error("Must pass in a block object to create a new fiber.");
            process.PushNil();
            return;
        }
        
        process.Push(Object::NewFiber(process.GetInterpreter(), args[0]));
    }
    
    PRIMITIVE(PrimitiveGetCurrentFiber)
    {
        process.Push(process.GetInterpreter().GetCurrentFiber());
    }
    
    PRIMITIVE(PrimitiveSwitchToFiber)
    {
        // get the fiber we're about switch to
        FiberObject * fiber = args[0]->AsFiber();
        
        // if you try to run a completed fiber, it does nothing
        if (fiber->GetProcess().IsDone())
        {
            process.PushNil();
            return;
        }
        
        // pass the value to the new fiber
        fiber->GetProcess().Push(args[1]);
        
        // run the new fiber
        process.GetInterpreter().SwitchToFiber(args[0]);
        
        // note that unlike other primitives, we are *not* pushing a result
        // onto the stack of the calling fiber here. when we switch back to
        // this fiber, we'll push the passed value on to the stack then.
    }
}

