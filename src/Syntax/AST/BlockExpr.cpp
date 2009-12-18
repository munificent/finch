#include <iostream>

#include "BlockExpr.h"

namespace Finch
{
    void BlockExpr::Trace(std::ostream & stream) const
    {
        stream << "{";
        
        if (mParams.size() > 0)
        {
            stream << "|";
            
            for (int i = 0; i < mParams.size(); i++)
            {
                stream << mParams[i];
                
                if(i < mParams.size() - 1) stream << " ";
            }            
            
            stream << "|";
        }
        
        stream << " " << mBody << " }";
    }
}