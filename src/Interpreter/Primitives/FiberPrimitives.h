#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    // Primitive methods for fibers.
    PRIMITIVE(FiberRunning);
    PRIMITIVE(FiberDone);
    PRIMITIVE(FiberRun);
    PRIMITIVE(FiberGetPrototype);
    PRIMITIVE(FiberNew);
    PRIMITIVE(FiberYield);
}


