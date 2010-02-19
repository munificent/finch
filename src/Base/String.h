#pragma once

#include <iostream>

#include "Ref.h"

namespace Finch
{
    using std::ostream;
    
    // Reference-counted heap-allocated immutable string class.
    class String
    {
    public:
        // Creates a new string using the given C-style format string and a
        // number of arguments to be formatted.
        static String Format(const String & format, ...);
        
        String() {}
        
        String(const char* chars);

        String(char c);

        String(const String & other)
        :   mData(other.mData)
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
        
        String Substring(int startIndex) const;
        String Substring(int startIndex, int count) const;
        
    private:
        struct StringData
        {
            StringData(const char * text)
            :   chars(text)
            {
                length = strlen(text);
            }
            
            int    length;
            const char * chars;
        };
        
        String(const String & left, const String & right);
        String(const char * text, bool isOnHeap);
        
        void Init(const char * text, bool isOnHeap);
        
        static const int FormattedStringMax = 512;
        
        static const char * sEmptyString;
        
        Ref<StringData> mData;
    };
    
    bool operator ==(const char * left, const String & right);
    bool operator !=(const char * left, const String & right);
    
    bool operator ==(const String & left, const char * right);
    bool operator !=(const String & left, const char * right);
    
    ostream & operator <<(ostream & cout, const String & string);
}