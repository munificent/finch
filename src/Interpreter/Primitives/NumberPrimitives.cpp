#include "NumberPrimitives.h"
#include "NumberObject.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    PRIMITIVE(NumberAdd)
    {
        interpreter.Push(Object::NewNumber(interpreter.GetEnvironment(), thisRef->AsNumber() + args[0]->AsNumber()));
    }

    PRIMITIVE(NumberSubtract)
    {
        interpreter.Push(Object::NewNumber(interpreter.GetEnvironment(), thisRef->AsNumber() - args[0]->AsNumber()));
    }
    
    PRIMITIVE(NumberMultiply)
    {
        interpreter.Push(Object::NewNumber(interpreter.GetEnvironment(), thisRef->AsNumber() * args[0]->AsNumber()));
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
            interpreter.Push(Object::NewNumber(interpreter.GetEnvironment(), thisRef->AsNumber() / dividend));
        }
    }

    PRIMITIVE(NumberAbs)
    {
        double number = thisRef->AsNumber();        
        double value = (number < 0) ? -number : number;
        interpreter.Push(Object::NewNumber(interpreter.GetEnvironment(), value));
    }

    PRIMITIVE(NumberNeg)
    {
        interpreter.Push(Object::NewNumber(interpreter.GetEnvironment(), -thisRef->AsNumber()));
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