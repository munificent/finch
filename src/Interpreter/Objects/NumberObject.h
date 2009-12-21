#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    using std::vector;
    
    // Object class for a number. All numbers in Finch are floating point.
    class NumberObject : public Object
    {
    public:
        NumberObject(Ref<Object> prototype, double value)
        :   Object(prototype),
            mValue(value)
        {}
        
        virtual void Trace(ostream & stream) const
        {
            stream << mValue;
        }
        
        virtual double AsNumber() const { return mValue; }
        
    private:
        double mValue;
    };    
}