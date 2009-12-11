#include <iostream>

#include "SequenceExpr.h"

namespace Finch
{
    void SequenceExpr::Trace(std::ostream & stream) const
    {
        stream << *mFirst << " ; " << *mSecond;
    }
}