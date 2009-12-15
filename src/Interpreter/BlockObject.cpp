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
                                     String message, const vector<Ref<Object> > & args)
    {
        // pass on to the block prototype
        return context.Block()->Receive(thisRef, context, message, args);
    }
}