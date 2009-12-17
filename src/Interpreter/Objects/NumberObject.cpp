#include "NumberObject.h"

namespace Finch
{
    void NumberObject::Trace(std::ostream & stream) const
    {
        stream << mValue;
    }
}