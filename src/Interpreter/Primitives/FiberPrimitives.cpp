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

}

