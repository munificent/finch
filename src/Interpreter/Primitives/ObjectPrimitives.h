#pragma once

#include <vector>

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    // Primitive methods for dynamic (i.e. regular) objects.
    PRIMITIVE(ObjectSelf);
    PRIMITIVE(ObjectEquals);
    PRIMITIVE(ObjectNotEquals);
    PRIMITIVE(ObjectCopy);
    PRIMITIVE(ObjectCopyWith);
    PRIMITIVE(ObjectAddMethodValue);
}