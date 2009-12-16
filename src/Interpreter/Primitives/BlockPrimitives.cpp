#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "EvalContext.h"

namespace Finch
{
    Ref<Object> BlockValue(Ref<Object> thisRef, EvalContext & context,
                           String message, const vector<Ref<Object> > & args)
    {
        BlockObject* block = thisRef->AsBlock();
        ASSERT_NOT_NULL(block);
        
        //### bob: need to handle block arguments too
        return context.EvaluateBlock(block->Body());
    }
}
