#include <math.h>

#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Process.h"

namespace Finch
{
    PRIMITIVE(NumberGetPrototype)
    {
        process.Push(process.GetEnvironment().NumberPrototype());
    }
    
    PRIMITIVE(NumberAdd)
    {
        process.PushNumber(thisRef->AsNumber() + args[0]->AsNumber());
    }

    PRIMITIVE(NumberSubtract)
    {
        process.PushNumber(thisRef->AsNumber() - args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberMultiply)
    {
        process.PushNumber(thisRef->AsNumber() * args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberDivide)
    {
        double dividend = args[0]->AsNumber();
        
        // check for divide by zero
        if (dividend == 0)
        {
            process.PushNil();
        }
        else
        {
            process.PushNumber(thisRef->AsNumber() / dividend);
        }
    }

    PRIMITIVE(NumberAbs)
    {
        double number = thisRef->AsNumber();        
        process.PushNumber((number < 0) ? -number : number);
    }

    PRIMITIVE(NumberNeg)
    {
        process.PushNumber(-thisRef->AsNumber());
    }
    
    PRIMITIVE(NumberSqrt)
    {
        process.PushNumber(sqrt(thisRef->AsNumber()));
    }
    
    PRIMITIVE(NumberSin)
    {
        process.PushNumber(sin(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberCos)
    {
        process.PushNumber(cos(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberTan)
    {
        process.PushNumber(tan(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAsin)
    {
        process.PushNumber(asin(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAcos)
    {
        process.PushNumber(acos(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAtan)
    {
        process.PushNumber(atan(thisRef->AsNumber()));
    }
    PRIMITIVE(NumberAtan2)
    {
        process.PushNumber(atan2(thisRef->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberMod)
    {
        process.PushNumber(fmod(thisRef->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberFloor)
    {
        process.PushNumber(floor(thisRef->AsNumber()));
    }
    
    PRIMITIVE(NumberCeiling)
    {
        process.PushNumber(ceil(thisRef->AsNumber()));
    }
    
    PRIMITIVE(NumberEquals)
    {
        process.PushBool(thisRef->AsNumber() == args[0]->AsNumber());
    }

    PRIMITIVE(NumberNotEquals)
    {
        process.PushBool(thisRef->AsNumber() != args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThan)
    {
        process.PushBool(thisRef->AsNumber() < args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThan)
    {
        process.PushBool(thisRef->AsNumber() > args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThanOrEqual)
    {
        process.PushBool(thisRef->AsNumber() <= args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThanOrEqual)
    {
        process.PushBool(thisRef->AsNumber() >= args[0]->AsNumber());
    }
}

