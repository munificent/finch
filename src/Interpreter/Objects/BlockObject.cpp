#include "BlockObject.h"
#include "BlockExpr.h"
#include "EvalContext.h"

namespace Finch
{
    void BlockObject::Trace(std::ostream & stream) const
    {
        stream << "block " << mBody;
    }
}