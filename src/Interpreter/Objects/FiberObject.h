#pragma once

#include <iostream>

#include "Macros.h"
#include "Fiber.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    class Interpreter;
        
    // Object class for a fiber.
    class FiberObject : public Object
    {
    public:
        FiberObject(const Value & parent, Interpreter & interpreter, const Value & block)
        :   Object(parent),
            mFiber(interpreter, block.Obj())
        {}
        
        virtual FiberObject * AsFiber() { return this; }
        
        Fiber & GetFiber() { return mFiber; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "fiber";
        }
        
    private:
        Fiber mFiber;
    };
}

