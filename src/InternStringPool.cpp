#include <stdio.h>

#include "InternStringPool.h"
#include "Macros.h"

namespace Finch
{
    InternString InternStringPool::Intern(const char * string)
    {
        ASSERT_NOT_NULL(string);
        
        //### bob: temp implementation!
        
        // see if we already have a matching string
        for (int i = 0; i < mCount; i++)
        {
            if (strcmp(string, mStrings[i].CString()) == 0)
            {
                return mStrings[i];
            }
        }
        
        // if we got here, the string wasn't found, so add it

        // hoist it onto the heap
        int length = strlen(string);
        char * heap = new char[length + 1];
        strcpy(heap, string);
        
        // add it to the pool
        mStrings[mCount++] = InternString(heap);
        return mStrings[mCount - 1];
    }
}