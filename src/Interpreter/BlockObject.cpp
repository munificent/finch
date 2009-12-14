#include "BlockObject.h"
#include "BlockExpr.h"
#include "EvalContext.h"

namespace Finch
{
    void BlockObject::Trace(std::ostream & stream) const
    {
        stream << "block " << mBody;
    }
    
    Ref<Object> BlockObject::Receive(Ref<Object> thisRef, EvalContext & context,
                                     String message, vector<Ref<Object> > args)
    {
        //### bob: this could be refactored into something more maintainable
        if ((message == "value") && (args.size() == 0))
        {
            return context.EvaluateBlock(mBody);
        }
        
        //### bob: need to handle block arguments too
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}