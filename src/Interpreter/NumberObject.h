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
        NumberObject(double value)
        :   mValue(value)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual Ref<Object> Receive(Ref<Object> thisRef, EvalContext & context,
                                    String message, vector<Ref<Object> > args);
        
        virtual double AsNumber() const { return mValue; }
        
    private:
        double mValue;
    };    
}