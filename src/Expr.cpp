#include "Expr.h"

namespace Finch
{
    std::ostream& operator<<(std::ostream& cout, const Expr & expr)
    {
        expr.Trace(cout);
        return cout;
    }        
}