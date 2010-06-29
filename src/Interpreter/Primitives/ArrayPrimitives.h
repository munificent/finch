#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    // Primitive methods for array objects.
    PRIMITIVE(ArrayGetPrototype);
    PRIMITIVE(ArrayLength);
    PRIMITIVE(ArrayAdd);
    PRIMITIVE(ArrayAt);
    PRIMITIVE(ArrayAtPut);
}

