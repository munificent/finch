#include <iostream>

#include "SymbolExpr.h"

namespace Finch
{
    void SymbolExpr::Trace(std::ostream & stream) const
    {
        stream << '`' << mValue;
    }
}