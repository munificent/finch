#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Fiber.h"

namespace Finch
{
    PRIMITIVE(BlockCall)
    {
        BlockObject * block = self.AsBlock();
        ASSERT_NOT_NULL(block);
        
        fiber.CallBlock(block->Self(), self, args);
        
        // This won't be used. Instead, when the called block unwinds, it will
        // set the result value into the appropriate register.
        return Value();
    }
}

