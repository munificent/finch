#include <iostream>

#include "BooleanPrimitives.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    PRIMITIVE(BooleanTrue)
    {
        interpreter.PushBool(true);
    }

    PRIMITIVE(BooleanFalse)
    {
        interpreter.PushBool(false);
    }
}
