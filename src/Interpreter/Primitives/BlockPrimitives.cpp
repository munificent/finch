#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"

namespace Finch
{
    Ref<Object> BlockCall(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        BlockObject* block = thisRef->AsBlock();
        ASSERT_NOT_NULL(block);
        
        return env.EvaluateBlock(block, args);
    }
}
