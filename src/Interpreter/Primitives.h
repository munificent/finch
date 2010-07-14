#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    // Built-in primitive methods on the special #PRIM# object. This defines
    // the core hard-coded functionality that all Finch behavior is defined in
    // terms of.
    PRIMITIVE(PrimitiveGetArrayPrototype);
    PRIMITIVE(PrimitiveGetBlockPrototype);
    PRIMITIVE(PrimitiveGetFiberPrototype);
    PRIMITIVE(PrimitiveGetNumberPrototype);
    PRIMITIVE(PrimitiveGetObjectPrototype);
    PRIMITIVE(PrimitiveGetStringPrototype);
    
    PRIMITIVE(PrimitiveCopy);
    PRIMITIVE(PrimitiveRunWithin);

    PRIMITIVE(PrimitiveStringConcat);
    PRIMITIVE(PrimitiveStringCompare);
    
    PRIMITIVE(PrimitiveWrite);
    
    PRIMITIVE(PrimitiveIfThenElse);
    PRIMITIVE(PrimitiveWhileDo);

    PRIMITIVE(PrimitiveNewFiber);
    PRIMITIVE(PrimitiveGetCurrentFiber);
    PRIMITIVE(PrimitiveSwitchToFiber);
}

