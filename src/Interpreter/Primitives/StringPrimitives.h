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
    
    // Primitive methods for string objects.
    PRIMITIVE(StringAdd);
    PRIMITIVE(StringLength);
    PRIMITIVE(StringAt);
    PRIMITIVE(StringEquals);
    PRIMITIVE(StringNotEquals);
    PRIMITIVE(StringHashCode);
}