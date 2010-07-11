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
    PRIMITIVE(ObjectEquals);
    PRIMITIVE(ObjectCopy);
    PRIMITIVE(ObjectCopyWith);
    PRIMITIVE(ObjectAddMethodBody);
    PRIMITIVE(ObjectGetParent);
    PRIMITIVE(ObjectSetParent);
}

