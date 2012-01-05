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
    PRIMITIVE(NumberAdd)
    {
        return fiber.CreateNumber(args[0]->AsNumber() + self->AsNumber());
    }

    PRIMITIVE(NumberSubtract)
    {
        return fiber.CreateNumber(args[0]->AsNumber() - self->AsNumber());
    }
    
    PRIMITIVE(NumberMultiply)
    {
        return fiber.CreateNumber(args[0]->AsNumber() * self->AsNumber());
    }
    
    PRIMITIVE(NumberDivide)
    {
        double dividend = self->AsNumber();
        
        // check for divide by zero
        if (dividend == 0)
        {
            return fiber.Nil();
        }
        else
        {
            return fiber.CreateNumber(args[0]->AsNumber() / dividend);
        }
    }

    PRIMITIVE(NumberAbs)
    {
        double number = self->AsNumber();
        return fiber.CreateNumber((number < 0) ? -number : number);
    }

    PRIMITIVE(NumberNeg)
    {
        return fiber.CreateNumber(-self->AsNumber());
    }
    
    PRIMITIVE(NumberSqrt)
    {
        return fiber.CreateNumber(sqrt(self->AsNumber()));
    }
    
    PRIMITIVE(NumberSin)
    {
        return fiber.CreateNumber(sin(self->AsNumber()));
    }
    PRIMITIVE(NumberCos)
    {
        return fiber.CreateNumber(cos(self->AsNumber()));
    }
    PRIMITIVE(NumberTan)
    {
        return fiber.CreateNumber(tan(self->AsNumber()));
    }
    PRIMITIVE(NumberAsin)
    {
        return fiber.CreateNumber(asin(self->AsNumber()));
    }
    PRIMITIVE(NumberAcos)
    {
        return fiber.CreateNumber(acos(self->AsNumber()));
    }
    PRIMITIVE(NumberAtan)
    {
        return fiber.CreateNumber(atan(self->AsNumber()));
    }
    PRIMITIVE(NumberAtan2)
    {
        return fiber.CreateNumber(atan2(self->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberMod)
    {
        return fiber.CreateNumber(fmod(self->AsNumber(), args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberFloor)
    {
        return fiber.CreateNumber(floor(self->AsNumber()));
    }
    
    PRIMITIVE(NumberCeiling)
    {
        return fiber.CreateNumber(ceil(self->AsNumber()));
    }
    
    PRIMITIVE(NumberEquals)
    {
        return fiber.CreateBool(self->AsNumber() == args[0]->AsNumber());
    }

    PRIMITIVE(NumberNotEquals)
    {
        return fiber.CreateBool(self->AsNumber() != args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThan)
    {
        return fiber.CreateBool(self->AsNumber() < args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThan)
    {
        return fiber.CreateBool(self->AsNumber() > args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberLessThanOrEqual)
    {
        return fiber.CreateBool(self->AsNumber() <= args[0]->AsNumber());
    }
    
    PRIMITIVE(NumberGreaterThanOrEqual)
    {
        return fiber.CreateBool(self->AsNumber() >= args[0]->AsNumber());
    }
}

