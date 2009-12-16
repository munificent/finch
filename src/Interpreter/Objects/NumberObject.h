#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    class NumberObject : public Object
    {
    public:
        NumberObject(Ref<Object> prototype, double value)
        :   Object(prototype),
            mValue(value)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual double AsNumber() const { return mValue; }
        
    private:
        double mValue;
    };    
}