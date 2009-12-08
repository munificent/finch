#include <iostream>

#include "KeywordExpr.h"

namespace Finch
{
    void KeywordExpr::Trace(std::ostream & stream) const
    {
        mObject->Trace(stream);
        
        for (int i = 0; i < mKeywords.size(); i++)
        {
            stream << " " << mKeywords[i] << ": ";
            mArgs[i]->Trace(stream);
        }
    }
}