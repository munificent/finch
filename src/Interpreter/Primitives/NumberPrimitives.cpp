#include <math.h>

#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Process.h"

namespace Finch
{
    PRIMITIVE(NumberAdd)
    {
        process.PushNumber(self->AsNumber() + args[0]->AsNumber());
    }

    PRIMITIVE(NumberSubtract)
    {
        process.PushNumber(self->AsNumber() - args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberMultiply)
    {
        process.PushNumber(self->AsNumber() * args[0]->AsNumber());
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
            process.PushNumber(self->AsNumber() / dividend);
        }
    }

    PRIMITIVE(NumberAbs)
    {
        double number = self->AsNumber();        
        process.PushNumber((number < 0) ? -number : number);
    }

    PRIMITIVE(NumberNeg)
    {
        process.PushNumber(-self->AsNumber());
    }
    
    PRIMITIVE(NumberSqrt)
    {
        process.PushNumber(sqrt(self->AsNumber()));
    }
    
    PRIMITIVE(NumberSin)
    {
        process.PushNumber(sin(self->AsNumber()));
    }
    PRIMITIVE(NumberCos)
    {
        process.PushNumber(cos(self->AsNumber()));
    }
    PRIMITIVE(NumberTan)
    {
        process.PushNumber(tan(self->AsNumber()));
    }
    PRIMITIVE(NumberAsin)
    {
        process.PushNumber(asin(self->AsNumber()));
    }
    PRIMITIVE(NumberAcos)
    {
        process.PushNumber(acos(self->AsNumber()));
    }
    PRIMITIVE(NumberAtan)
    {
        process.PushNumber(atan(self->AsNumber()));
    }
    PRIMITIVE(NumberAtan2)
    {
        process.PushNumber(atan2(self->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberMod)
    {
        process.PushNumber(fmod(self->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberFloor)
    {
        process.PushNumber(floor(self->AsNumber()));
    }
    
    PRIMITIVE(NumberCeiling)
    {
        process.PushNumber(ceil(self->AsNumber()));
    }
    
    PRIMITIVE(NumberEquals)
    {
        process.PushBool(self->AsNumber() == args[0]->AsNumber());
    }

    PRIMITIVE(NumberNotEquals)
    {
        process.PushBool(self->AsNumber() != args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThan)
    {
        process.PushBool(self->AsNumber() < args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThan)
    {
        process.PushBool(self->AsNumber() > args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThanOrEqual)
    {
        process.PushBool(self->AsNumber() <= args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThanOrEqual)
    {
        process.PushBool(self->AsNumber() >= args[0]->AsNumber());
    }
}

