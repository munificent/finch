#include <iostream>

#include "UnaryExpr.h"

namespace Finch
{
    void UnaryExpr::Trace(std::ostream & stream) const
    {
        mObject->Trace(stream);
        stream << " " << mMessage;
    }
}