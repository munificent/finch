#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    PRIMITIVE(BlockGetPrototype)
    {
        interpreter.Push(interpreter.GetEnvironment().BlockPrototype());
    }
    
    PRIMITIVE(BlockCall)
    {
        ASSERT_NOT_NULL(thisRef->AsBlock());
        interpreter.CallBlock(thisRef, args);
    }
}

