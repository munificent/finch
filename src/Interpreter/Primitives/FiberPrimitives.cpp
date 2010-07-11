#include <iostream>

#include "FiberPrimitives.h"
#include "FiberObject.h"
#include "Environment.h"
#include "Interpreter.h"
#include "Process.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(FiberRunning)
    {
        FiberObject * fiber = self->AsFiber();
        process.PushBool(fiber->GetProcess().IsRunning());
    }
    
    PRIMITIVE(FiberDone)
    {
        FiberObject * fiber = self->AsFiber();
        process.PushBool(fiber->GetProcess().IsDone());
    }
    
    PRIMITIVE(FiberRun)
    {
        // get the fiber we're about switch to
        FiberObject * fiber = self->AsFiber();

        // if you try to run a completed process, it does nothing
        if (fiber->GetProcess().IsDone())
        {
            process.PushNil();
            return;
        }
        
        // note what fiber the fiber we're about to start will yield to
        Ref<Object> currentFiber = process.GetInterpreter().GetCurrentFiber();
        fiber->GetProcess().SetCallingFiber(currentFiber);
        
        // pass the argument to run: over to the new fiber
        fiber->GetProcess().Push(args[0]);
        
        // run the new fiber
        process.GetInterpreter().SwitchToFiber(self);
        
        // note that unlike other primitives, we are *not* pushing a result
        // onto the stack here. that's because this fiber is about to stop
        // processing, before the call to "run" has completed. when that fiber
        // is resumed by the calling fiber yielding, the yield method will
        // ensure that the resumed fiber gets a result pushed before it starts
        // up and completes the run method.
    }
    
    PRIMITIVE(FiberGetPrototype)
    {
        process.Push(process.GetEnvironment().FiberPrototype());
    }
    
    PRIMITIVE(FiberNew)
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

    PRIMITIVE(FiberYield)
    {
        // get the fiber that's going to yield
        Ref<Object> currentFiberObj = process.GetInterpreter().GetCurrentFiber();
        FiberObject * currentFiber = currentFiberObj->AsFiber();
        
        // get the fiber that we're going to resume (yield to)
        Ref<Object> newFiberObj = currentFiber->GetProcess().GetCallingFiber();
        FiberObject * newFiber = newFiberObj->AsFiber();
        
        // give the resuming fiber the yielded value
        newFiber->GetProcess().Push(args[0]);
        
        // resume the old fiber
        process.GetInterpreter().SwitchToFiber(newFiberObj);

        // note that we're not pushing a return value for yield onto the
        // yielding fiber's stack. much like run, we'll handle that right
        // before the yielding fiber is resumed.
    }
}

