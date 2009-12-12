#include <iostream>

#include "NumberExpr.h"

namespace Finch
{
    void NumberExpr::Trace(std::ostream & stream) const
    {
        stream << mValue;
    }
}