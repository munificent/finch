#include "BlockObject.h"
#include "BlockExpr.h"
#include "Evaluator.h"

namespace Finch
{
    void BlockObject::Trace(std::ostream & stream) const
    {
        stream << "block " << mBody;
    }
    
    Ref<Object> BlockObject::Receive(Ref<Object> thisRef, String message, vector<Ref<Object> > args)
    {
        //### bob: this could be refactored into something more maintainable
        if ((message == "value") && (args.size() == 0))
        {
            //### bob: looking up nil here is kind of lame
            Evaluator evaluator(mParentScope, mParentScope->LookUp("Nil"));
            return evaluator.Evaluate(mBody);
        }
        
        //### bob: need to handle block arguments too
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}