#include <iostream>

#include "NameExpr.h"

namespace Finch
{
    void NameExpr::Trace(std::ostream & stream) const
    {
        stream << mName;
    }
}