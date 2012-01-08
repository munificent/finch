#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Fiber.h"

namespace Finch
{
    PRIMITIVE(BlockCall)
    {
        BlockObject * block = self->AsBlock();
        ASSERT_NOT_NULL(block);
        
        fiber.CallBlock(block->Self(), self, args);
    }
}

