#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    // Primitive methods for string objects.
    PRIMITIVE(StringLength);
    PRIMITIVE(StringAt);
    PRIMITIVE(StringHashCode);
}

