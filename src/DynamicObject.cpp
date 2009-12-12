#include "DynamicObject.h"

namespace Finch
{
    void DynamicObject::Trace(std::ostream & stream) const
    {
        stream << "object";
    }
    
    Ref<Object> DynamicObject::Receive(String message, vector<Ref<Object> > args)
    {
        if (message == "copy")
        {
            //### bob: temp. just make a new empty object. should actually
            // make a new one with its prototype set to this
            return Object::New();
        }
        else if (message == "addField:value:")
        {
            //### bob: need symbol literals and objects before i can implement this!
        }
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}