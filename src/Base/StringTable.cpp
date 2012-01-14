#include "StringTable.h"

namespace Finch
{
    StringId StringTable::Add(const String & string)
    {
        // See if the string is already in the table. We must ensure each string
        // only appears once in the table so that we can reliably compare
        // strings just by index.
        // TODO(bob): This is O(n). Optimize using a hashtable.
        for (int i = 0; i < mStrings.Count(); i++)
        {
            if (mStrings[i] == string) return i;
        }

        // Not in the table, so add it.
        mStrings.Add(string);
        return mStrings.Count() - 1;
    }
    
    String StringTable::Find(StringId id)
    {
        return mStrings[id];
    }
}

