#include <iostream>

#include "BlockPrimitives.h"
#include "BlockObject.h"
#include "Environment.h"

namespace Finch
{
    Ref<Object> NilWhileDo(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        BlockObject* condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return env.Nil();
        
        vector<Ref<Object> > noArgs;
        
        while (env.EvaluateBlock(condition->Body()) == env.True())
        {
            args[1]->Receive(args[1], env, "value", noArgs);
        }
        
        return Ref<Object>();
    }
    
    Ref<Object> NilIfThen(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        BlockObject* condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return env.Nil();
        
        vector<Ref<Object> > noArgs;
        
        if (env.EvaluateBlock(condition->Body()) == env.True())
        {
            return args[1]->Receive(args[1], env, "value", noArgs);
        }
        
        return Ref<Object>();
    }
    
    Ref<Object> NilIfThenElse(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args)
    {
        BlockObject* condition = args[0]->AsBlock();
        
        // do nothing if the condition isn't a block
        //### bob: report error here?
        if (condition == NULL) return env.Nil();
        
        vector<Ref<Object> > noArgs;
        
        if (env.EvaluateBlock(condition->Body()) == env.True())
        {
            return args[1]->Receive(args[1], env, "value", noArgs);
        }
        else
        {
            return args[2]->Receive(args[2], env, "value", noArgs);
        }
    }
}
