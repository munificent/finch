#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    PRIMITIVE(PrimitiveStringConcat);
    PRIMITIVE(PrimitiveStringCompare);
    
    PRIMITIVE(PrimitiveWrite);
    
    PRIMITIVE(PrimitiveNewFiber);
    PRIMITIVE(PrimitiveGetCurrentFiber);
    PRIMITIVE(PrimitiveSwitchToFiber);
    PRIMITIVE(PrimitiveGetCallstackDepth);
}

