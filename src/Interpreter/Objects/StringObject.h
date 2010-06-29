#pragma once

#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // Object class for a string.
    class StringObject : public Object
    {
    public:
        StringObject(Ref<Object> parent, String value)
        :   Object(parent),
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

