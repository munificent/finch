#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::ostream;
    using std::stringstream;
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
        virtual String AsString() const
        {
            std::stringstream result;
            result << mValue;
            return result.str();
        }
        
    private:
        double mValue;
    };    
}