#pragma once

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    // Primitive methods for dynamic (i.e. regular) objects.
    PRIMITIVE(ObjectGetPrototype);
    PRIMITIVE(ObjectToString);
    PRIMITIVE(ObjectEquals);
    PRIMITIVE(ObjectNotEquals);
    PRIMITIVE(ObjectCopy);
    PRIMITIVE(ObjectCopyWith);
    PRIMITIVE(ObjectAddMethodBody);
    PRIMITIVE(ObjectGetParent);
    PRIMITIVE(ObjectSetParent);
}

