#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "EvalContext.h"

namespace Finch
{
    Ref<Object> NilWhileDo(Ref<Object> thisRef, EvalContext & context,
                           String message, const vector<Ref<Object> > & args)
    {
        BlockObject* condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return context.Nil();
        
        vector<Ref<Object> > noArgs;
        
        while (context.EvaluateBlock(condition->Body()) == context.True())
        {
            args[1]->Receive(args[1], context, "value", noArgs);
        }
        
        return Ref<Object>();
    }
    
    Ref<Object> NilIfThen(Ref<Object> thisRef, EvalContext & context,
                           String message, const vector<Ref<Object> > & args)
    {
        BlockObject* condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return context.Nil();
        
        vector<Ref<Object> > noArgs;
        
        if (context.EvaluateBlock(condition->Body()) == context.True())
        {
            return args[1]->Receive(args[1], context, "value", noArgs);
        }
        
        return Ref<Object>();
    }
    
    Ref<Object> NilIfThenElse(Ref<Object> thisRef, EvalContext & context,
                          String message, const vector<Ref<Object> > & args)
    {
        BlockObject* condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return context.Nil();
        
        vector<Ref<Object> > noArgs;
        
        if (context.EvaluateBlock(condition->Body()) == context.True())
        {
            return args[1]->Receive(args[1], context, "value", noArgs);
        }
        else
        {
            return args[2]->Receive(args[2], context, "value", noArgs);
        }
    }
}
