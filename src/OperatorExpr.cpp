#include <iostream>

#include "OperatorExpr.h"

namespace Finch
{
    void OperatorExpr::Trace(std::ostream & stream) const
    {
        mObject->Trace(stream);
        stream << " " << mOperator << " ";
        mArg->Trace(stream);
    }
}