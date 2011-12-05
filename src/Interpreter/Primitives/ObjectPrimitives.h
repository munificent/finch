#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    // Primitive methods for dynamic (i.e. regular) objects.
    PRIMITIVE(ObjectToString);
    PRIMITIVE(ObjectSame);
    PRIMITIVE(ObjectGetParent);
    PRIMITIVE(ObjectSetParent);
}

