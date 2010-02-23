#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
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