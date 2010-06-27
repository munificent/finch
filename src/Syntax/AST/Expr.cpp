#include "Expr.h"

namespace Finch
{
    using std::ostream;
    
    ostream & operator<<(ostream & cout, const Expr & expr)
    {
        expr.Trace(cout);
        return cout;
    }
}

