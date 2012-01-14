#pragma once

#include "Array.h"
#include "FinchString.h"

namespace Finch
{
    // A string table for interning strings and allowing them to be looked up
    // by id. Used by the bytecode compiler to reference string literals from
    // code.
    class StringTable
    {
    public:
        // Adds the given string to the table if not already present, and
        // returns its ID.
        StringId Add(const String & string);
        
        // Looks up the string with the given ID in the table.
        String Find(StringId id);
        
    private:
        Array<String> mStrings;
    };
}

