#include "BlockObject.h"
#include "BlockExpr.h"
#include "Evaluator.h"

namespace Finch
{
    void BlockObject::Trace(std::ostream & stream) const
    {
        stream << "block " << mBody;
    }
    
    Ref<Object> BlockObject::Receive(String message, vector<Ref<Object> > args)
    {
        //### bob: this could be refactored into something more maintainable
        if ((message == "value") && (args.size() == 0))
        {
            Evaluator evaluator(mParentScope);
            return evaluator.Evaluate(mBody);
        }
        
        //### bob: need to handle block arguments too
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}