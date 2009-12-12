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
        StringObject(String value)
        :   mValue(value)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual Ref<Object> Receive(Ref<Object> thisRef, String message, vector<Ref<Object> > args);
        
        virtual String AsString() const { return mValue; }
        
    private:
        String mValue;
    };    
}