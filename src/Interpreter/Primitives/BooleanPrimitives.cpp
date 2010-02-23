#include <iostream>

#include "BooleanPrimitives.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    PRIMITIVE(BooleanTrue)
    {
        interpreter.Push(interpreter.GetEnvironment().True());
    }

    PRIMITIVE(BooleanFalse)
    {
        interpreter.Push(interpreter.GetEnvironment().False());
    }
}
