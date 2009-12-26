#pragma once

#include <iostream>

#include "ArrayRef.h"

namespace Finch
{
    using std::ostream;
    
    //### bob: to do:
    // - instead of ArrayRef, have String store a Ref to a HeapString object
    //   which owns the character array, but also caches length and hash code?
    // - interning support
    
    // Reference-counted heap-allocated immutable string class.
    class String
    {
    public:
        static String Format(const String & format, ...);
        
        String() {}
        
        String(const char* chars);

        String(char c);

        String(const String & other)
        :   mChars(other.mChars)
        {}
        
        bool operator <(const String & other) const;
        bool operator <=(const String & other) const;
        bool operator >(const String & other) const;
        bool operator >=(const String & other) const;
        
        bool operator ==(const String & other) const;
        bool operator !=(const String & other) const;
        
        const char & operator[] (int index) const;
        
        String operator +(const String & other) const;
        String & operator +=(const String & other);
        
        const char* CString() const;

        int Length() const;
        
    private:
        String(const String & left, const String & right);
        
        static const int FormattedStringMax = 512;
        
        static const char * sEmptyString;
        
        ArrayRef<char> mChars;
    };
    
    bool operator ==(const char * left, const String & right);
    bool operator !=(const char * left, const String & right);
    
    bool operator ==(const String & left, const char * right);
    bool operator !=(const String & left, const char * right);
    
    ostream & operator <<(ostream & cout, const String & string);
}