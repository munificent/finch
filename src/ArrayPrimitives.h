#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    // Primitive methods for array objects.
    PRIMITIVE(ArrayLength);
    PRIMITIVE(ArrayAdd);
    PRIMITIVE(ArrayAt);
    PRIMITIVE(ArrayAtPut);
    PRIMITIVE(ArrayNew);
    PRIMITIVE(ArrayNewWith);
}