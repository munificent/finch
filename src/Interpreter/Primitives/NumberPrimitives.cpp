#include <math.h>

#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Fiber.h"

namespace Finch
{
    // The binary operators here are the result of a double-dispatch on the two
    // operands. When you do "1 - 2", it sends a "-" message to 1 which in turn
    // sends "-number:" to 2, passing in itself as the argument. The primitives
    // here are those second messages. That means the operands are reversed:
    // self is the RHS and the arg is the LHS.
    /*
    PRIMITIVE(NumberAdd)
    {
        fiber.PushNumber(args[0]->AsNumber() + self->AsNumber());
    }

    PRIMITIVE(NumberSubtract)
    {
        fiber.PushNumber(args[0]->AsNumber() - self->AsNumber());
    }
    
    PRIMITIVE(NumberMultiply)
    {
        fiber.PushNumber(args[0]->AsNumber() * self->AsNumber());
    }
    
    PRIMITIVE(NumberDivide)
    {
        double dividend = self->AsNumber();
        
        // check for divide by zero
        if (dividend == 0)
        {
            fiber.PushNil();
        }
        else
        {
            fiber.PushNumber(args[0]->AsNumber() / dividend);
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
     */
}

