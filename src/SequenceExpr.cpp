#include <iostream>

#include "SequenceExpr.h"

namespace Finch
{
    void SequenceExpr::Trace(std::ostream & stream) const
    {
        mFirst->Trace(stream);
        stream << " . ";
        mSecond->Trace(stream);
    }
}