#include <iostream>

#include "FiberPrimitives.h"
#include "FiberObject.h"
#include "Interpreter.h"
#include "Fiber.h"
#include "Object.h"

namespace Finch
{
    PRIMITIVE(FiberRunning)
    {
        FiberObject * fiberObj = self.AsFiber();
        return fiber.CreateBool(fiberObj->GetFiber().IsRunning());
    }
    
    PRIMITIVE(FiberDone)
    {
        FiberObject * fiberObj = self.AsFiber();
        return fiber.CreateBool(fiberObj->GetFiber().IsDone());
    }
}

