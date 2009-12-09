#include <iostream>

#include "BlockExpr.h"

namespace Finch
{
    void BlockExpr::Trace(std::ostream & stream) const
    {
        stream << "{";
        
        if (mArgs.size() > 0)
        {
            stream << "|";
            
            for (int i = 0; i < mArgs.size(); i++)
            {
                stream << mArgs[i];
                
                if(i < mArgs.size() - 1) stream << " ";
            }            
            
            stream << "|";
        }
        
        stream << " ";
        
        mBody->Trace(stream);
        
        stream << " }";
    }
}