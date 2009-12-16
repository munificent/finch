#include "StringObject.h"

namespace Finch
{
    void StringObject::Trace(std::ostream & stream) const
    {
        stream << "\"" << mValue << "\"";
    }
}