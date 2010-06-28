#include "StringTable.h"

namespace Finch
{
    int StringTable::Add(String string)
    {
        // see if the string is already in the table
		//### bob: this is O(n). could optimize using a hashtable.
        for (int i = 0; i < mStrings.Count(); i++)
        {
            if (mStrings[i] == string) return i;
        }

        // not in the table, so add it
        mStrings.Add(string);
        return mStrings.Count() - 1;
    }
    
    String StringTable::Find(int id)
    {
        return mStrings[id];
    }
}

