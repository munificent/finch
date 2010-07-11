#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Process.h"

namespace Finch
{
    PRIMITIVE(BlockGetPrototype)
    {
        process.Push(process.GetEnvironment().BlockPrototype());
    }
    
    PRIMITIVE(BlockCall)
    {
        ASSERT_NOT_NULL(self->AsBlock());
        process.CallBlock(self, args);
    }
}

