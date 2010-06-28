#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    // Primitive methods for numbers.
    PRIMITIVE(NumberAdd);
    PRIMITIVE(NumberSubtract);
    PRIMITIVE(NumberMultiply);
    PRIMITIVE(NumberDivide);
    PRIMITIVE(NumberAbs);
    PRIMITIVE(NumberNeg);
    
    PRIMITIVE(NumberSqrt);
    PRIMITIVE(NumberSin);
    PRIMITIVE(NumberCos);
    PRIMITIVE(NumberTan);
    PRIMITIVE(NumberAsin);
    PRIMITIVE(NumberAcos);
    PRIMITIVE(NumberAtan);
    PRIMITIVE(NumberAtan2);
    
    PRIMITIVE(NumberMod);
    PRIMITIVE(NumberFloor);
    PRIMITIVE(NumberCeiling);
    
    PRIMITIVE(NumberEquals);
    PRIMITIVE(NumberNotEquals);
    PRIMITIVE(NumberLessThan);
    PRIMITIVE(NumberGreaterThan);
    PRIMITIVE(NumberLessThanOrEqual);
    PRIMITIVE(NumberGreaterThanOrEqual);
}

