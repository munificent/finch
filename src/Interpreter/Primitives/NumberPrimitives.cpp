#include <math.h>

#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Fiber.h"

namespace Finch
{
    PRIMITIVE(NumberAdd)
    {
        fiber.PushNumber(self->AsNumber() + args[0]->AsNumber());
    }

    PRIMITIVE(NumberSubtract)
    {
        fiber.PushNumber(self->AsNumber() - args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberMultiply)
    {
        fiber.PushNumber(self->AsNumber() * args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberDivide)
    {
        double dividend = args[0]->AsNumber();
        
        // check for divide by zero
        if (dividend == 0)
        {
            fiber.PushNil();
        }
        else
        {
            fiber.PushNumber(self->AsNumber() / dividend);
        }
    }

    PRIMITIVE(NumberAbs)
    {
        double number = self->AsNumber();        
        fiber.PushNumber((number < 0) ? -number : number);
    }

    PRIMITIVE(NumberNeg)
    {
        fiber.PushNumber(-self->AsNumber());
    }
    
    PRIMITIVE(NumberSqrt)
    {
        fiber.PushNumber(sqrt(self->AsNumber()));
    }
    
    PRIMITIVE(NumberSin)
    {
        fiber.PushNumber(sin(self->AsNumber()));
    }
    PRIMITIVE(NumberCos)
    {
        fiber.PushNumber(cos(self->AsNumber()));
    }
    PRIMITIVE(NumberTan)
    {
        fiber.PushNumber(tan(self->AsNumber()));
    }
    PRIMITIVE(NumberAsin)
    {
        fiber.PushNumber(asin(self->AsNumber()));
    }
    PRIMITIVE(NumberAcos)
    {
        fiber.PushNumber(acos(self->AsNumber()));
    }
    PRIMITIVE(NumberAtan)
    {
        fiber.PushNumber(atan(self->AsNumber()));
    }
    PRIMITIVE(NumberAtan2)
    {
        fiber.PushNumber(atan2(self->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberMod)
    {
        fiber.PushNumber(fmod(self->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberFloor)
    {
        fiber.PushNumber(floor(self->AsNumber()));
    }
    
    PRIMITIVE(NumberCeiling)
    {
        fiber.PushNumber(ceil(self->AsNumber()));
    }
    
    PRIMITIVE(NumberEquals)
    {
        fiber.PushBool(self->AsNumber() == args[0]->AsNumber());
    }

    PRIMITIVE(NumberNotEquals)
    {
        fiber.PushBool(self->AsNumber() != args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThan)
    {
        fiber.PushBool(self->AsNumber() < args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThan)
    {
        fiber.PushBool(self->AsNumber() > args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThanOrEqual)
    {
        fiber.PushBool(self->AsNumber() <= args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThanOrEqual)
    {
        fiber.PushBool(self->AsNumber() >= args[0]->AsNumber());
    }
}

