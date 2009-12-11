#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    class Object
    {
    public:
        // virtual constructors
        static Ref<Object> New(double value);
        
        virtual ~Object() {}
        
        virtual Ref<Object> Receive(String message, vector<Ref<Object> > args) = 0;
        
        virtual double AsNumber() const = 0;
        
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    std::ostream& operator<<(std::ostream& cout, const Object & object);
}