#include <iostream>

#include "SetExpr.h"

namespace Finch
{
    void SetExpr::Trace(std::ostream & stream) const
    {
        stream << "set " << mName << " " << mValue;
    }
}