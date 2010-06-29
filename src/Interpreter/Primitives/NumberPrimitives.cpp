#include <math.h>

#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    PRIMITIVE(NumberGetPrototype)
    {
        interpreter.Push(interpreter.GetEnvironment().NumberPrototype());
    }
    
    PRIMITIVE(NumberAdd)
    {
        interpreter.PushNumber(thisRef->AsNumber() + args[0]->AsNumber());
    }

    PRIMITIVE(NumberSubtract)
    {
        interpreter.PushNumber(thisRef->AsNumber() - args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberMultiply)
    {
        interpreter.PushNumber(thisRef->AsNumber() * args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberDivide)
    {
        double dividend = args[0]->AsNumber();
        
        // check for divide by zero
        if (dividend == 0)
        {
            interpreter.PushNil();
        }
        else
        {
            interpreter.PushNumber(thisRef->AsNumber() / dividend);
        }
    }

    PRIMITIVE(NumberAbs)
    {
        double number = thisRef->AsNumber();        
        interpreter.PushNumber((number < 0) ? -number : number);
    }

    PRIMITIVE(NumberNeg)
    {
        interpreter.PushNumber(-thisRef->AsNumber());
    }
    
    PRIMITIVE(NumberSqrt)
    {
        interpreter.PushNumber(sqrt(thisRef->AsNumber()));
    }
    
    PRIMITIVE(NumberSin)
    {
        interpreter.PushNumber(sin(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberCos)
    {
        interpreter.PushNumber(cos(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberTan)
    {
        interpreter.PushNumber(tan(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAsin)
    {
        interpreter.PushNumber(asin(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAcos)
    {
        interpreter.PushNumber(acos(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAtan)
    {
        interpreter.PushNumber(atan(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAtan2)
    {
        interpreter.PushNumber(atan2(thisRef->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberMod)
    {
        interpreter.PushNumber(fmod(thisRef->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberFloor)
    {
        interpreter.PushNumber(floor(thisRef->AsNumber()));
    }
    
    PRIMITIVE(NumberCeiling)
    {
        interpreter.PushNumber(ceil(thisRef->AsNumber()));
    }
    
    PRIMITIVE(NumberEquals)
    {
        interpreter.PushBool(thisRef->AsNumber() == args[0]->AsNumber());
    }

    PRIMITIVE(NumberNotEquals)
    {
        interpreter.PushBool(thisRef->AsNumber() != args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThan)
    {
        interpreter.PushBool(thisRef->AsNumber() < args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThan)
    {
        interpreter.PushBool(thisRef->AsNumber() > args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThanOrEqual)
    {
        interpreter.PushBool(thisRef->AsNumber() <= args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThanOrEqual)
    {
        interpreter.PushBool(thisRef->AsNumber() >= args[0]->AsNumber());
    }
}

