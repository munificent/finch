#pragma once

#include "String.h"

namespace Finch
{
    //### bob: the implementation of this just a temp prototype.
    class StringTable
    {
    public:
        // Adds the given string to the table if not already present, and
        // returns its ID.
        int Add(String string);
        
        // Looks up the string with the given ID in the table.
        String Find(int id);
        
    private:
        static const int MAX_STRINGS = 4096; //### bob: arbitrary
        
        String mStrings[MAX_STRINGS];
        int    mNumStrings;
    };
}