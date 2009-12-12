#include <iostream>

#include "OperatorExpr.h"

namespace Finch
{
    void OperatorExpr::Trace(std::ostream & stream) const
    {
        stream << mReceiver << " " << mOperator << " " << mArg;
    }
}