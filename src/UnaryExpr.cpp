#include <iostream>

#include "UnaryExpr.h"

namespace Finch
{
    void UnaryExpr::Trace(std::ostream & stream) const
    {
        stream << mReceiver << " " << mMessage;
    }
}