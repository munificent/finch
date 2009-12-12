#include "NumberObject.h"

namespace Finch
{
    void NumberObject::Trace(std::ostream & stream) const
    {
        stream << mValue;
    }

    Ref<Object> NumberObject::Receive(String message, vector<Ref<Object> > args)
    {
        //### bob: this could be refactored into something more maintainable
        if (message == "abs")
        {
            double value = (mValue < 0) ? -mValue : mValue;
            return Object::New(value); 
        }
        else if (message == "neg")
        {
            return Object::New(-mValue); 
        }
        else if (message == "+")
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue + value); 
        }
        else if (message == "-")
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue - value); 
        }
        else if (message == "*")
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue * value); 
        }
        else if (message == "/")
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue / value); 
        }
        //### bob: temp!
        else if (message == "hack:temp:")
        {
            double value1 = args[0]->AsNumber();
            double value2 = args[1]->AsNumber();
            return Object::New(mValue + value1 + value2); 
        }
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}