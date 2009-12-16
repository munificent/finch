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
    
    class StringObject : public Object
    {
    public:
        StringObject(Ref<Object> prototype, String value)
        :   Object(prototype),
            mValue(value)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual String AsString() const { return mValue; }
        
    private:
        String mValue;
    };    
}