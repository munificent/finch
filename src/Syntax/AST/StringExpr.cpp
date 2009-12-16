#include <iostream>

#include "StringExpr.h"

namespace Finch
{
    void StringExpr::Trace(std::ostream & stream) const
    {
        stream << '\"' << mValue << '\"';
    }
}