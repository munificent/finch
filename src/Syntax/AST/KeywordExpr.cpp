#include <iostream>

#include "KeywordExpr.h"

namespace Finch
{
    void KeywordExpr::Trace(std::ostream & stream) const
    {
        stream << mReceiver;
        
        for (int i = 0; i < mKeywords.size(); i++)
        {
            stream << " " << mKeywords[i] << " " << mArgs[i];
        }
    }
}