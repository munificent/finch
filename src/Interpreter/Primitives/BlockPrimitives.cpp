#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    PRIMITIVE(BlockCall)
    {
        BlockObject * block = thisRef->AsBlock();
        ASSERT_NOT_NULL(block);
        
        interpreter.CallBlock(*block, args);
    }
}
