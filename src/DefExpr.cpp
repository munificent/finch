#include <iostream>

#include "DefExpr.h"

namespace Finch
{
    void DefExpr::Trace(std::ostream & stream) const
    {
        stream << "def " << mName << " " << mValue;
    }
}