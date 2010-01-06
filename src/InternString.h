#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    using std::ostream;
    
    //### bob: lots of duplication between this and String. refactor to be
    // polymorphic?
    
    // An immutable string in the intern string pool. This class is basically
    // a wrapper around a char pointer. It provides type-safety (it's clear
    // who owns the wrapped string) and some convenience methods.
    class InternString
    {
        friend class InternStringPool;
        
    public:
        InternString()
        :   mChars(NULL)
        {}
        
        const char * CString() const { return mChars; }
        
        bool operator ==(const InternString & other) const { return mChars == other.mChars; }
        bool operator !=(const InternString & other) const { return mChars != other.mChars; }
        
    protected:
        InternString(const char * chars)
        :   mChars(chars)
        {
            ASSERT_NOT_NULL(chars);
        }
        
    private:
        const char * mChars;
    };
    
    ostream & operator <<(ostream & cout, const InternString & string);
}