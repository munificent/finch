#include "InternString.h"
#include "Macros.h"

namespace Finch
{
    ostream & operator <<(ostream & cout, const InternString & string)
    {
        cout << string.CString();
        return cout;
    }
}