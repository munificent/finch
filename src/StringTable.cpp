/*
 *  StringTable.cpp
 *  finch
 *
 *  Created by Robert Nystrom on 2/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "StringTable.h"

namespace Finch
{
    int StringTable::Add(String string)
    {
        //### bob: doesn't actually check for duplicates yet
        mStrings[mNumStrings] = string;
        
        return mNumStrings++;
    }
    
    String StringTable::Find(int id)
    {
        ASSERT_INDEX(id, mNumStrings);
        
        return mStrings[id];
    }
}
