#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Fiber.h"

namespace Finch
{
    PRIMITIVE(BlockCall)
    {
        ASSERT_NOT_NULL(self->AsBlock());
        fiber.CallBlock(self, args);
    }
}

