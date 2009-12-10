#include <iostream>

#include "UnaryExpr.h"

namespace Finch
{
    void UnaryExpr::Trace(std::ostream & stream) const
    {
        mReceiver->Trace(stream);
        stream << " " << mMessage;
    }
}