#pragma once

#include <iostream>

#include "InternString.h"
#include "Macros.h"

namespace Finch
{
    using std::ostream;
    
    class InternString;
    
    //### bob: lots of duplication between this and String. refactor to be
    // polymorphic?
    
    // A pool of interned strings. All string literals appearing in code are
    // added to this pool and never removed. The pool automatically detects and
    // combines duplicates. This way, equality of two interned strings can be
    // determined simply by comparing pointers.
    class InternStringPool
    {
    public:
        InternStringPool()
        :   mCount(0)
        {}
        
        // Interns the given string in the pool. If the pool doesn't already
        // contain the given string, a copy of it will be added to the pool.
        // Otherwise, the existing one is used. Returns the interned string.
        InternString Intern(const char * string);
        
    private:
        //### bob: temp
        static const int MAX_INTERN_STRINGS = 1000;
        int mCount;
        InternString mStrings[MAX_INTERN_STRINGS];
    };
}