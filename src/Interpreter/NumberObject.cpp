#include "NumberObject.h"
#include "EvalContext.h"

namespace Finch
{
    void NumberObject::Trace(std::ostream & stream) const
    {
        stream << mValue;
    }

    Ref<Object> NumberObject::Receive(Ref<Object> thisRef, EvalContext & context,
                                      String message, const vector<Ref<Object> > & args)
    {
        return context.Number()->Receive(thisRef, context, message, args);
    }
}