#pragma once

#include <iostream>

#include "Macros.h"
#include "Process.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    class Interpreter;
        
    // Object class for a fiber.
    class FiberObject : public Object
    {
    public:
        FiberObject(Ref<Object> parent, Interpreter & interpreter, Ref<Object> block)
        :   Object(parent),
            mProcess(interpreter, block)        {}
        
        virtual FiberObject * AsFiber() { return this; }
        
        Process & GetProcess() { return mProcess; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "fiber";
        }
        
    private:
        Process mProcess;
    };
}

