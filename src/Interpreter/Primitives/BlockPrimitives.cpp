#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Process.h"

namespace Finch
{
    PRIMITIVE(BlockCall)
    {
        ASSERT_NOT_NULL(self->AsBlock());
        process.CallBlock(self, args);
    }
}

