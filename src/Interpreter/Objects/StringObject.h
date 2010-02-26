#pragma once

#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    
    // Object class for a string.
    class StringObject : public Object
    {
    public:
        StringObject(Ref<Object> prototype, String value)
        :   Object(prototype),
            mValue(value)
        {}
        
        virtual void Trace(ostream & stream) const
        {
            stream << "\"" << mValue << "\"";
        }
            
        virtual String AsString() const { return mValue; }
        
    private:
        String mValue;
    };    
}