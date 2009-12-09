#include "Value.h"
#include "Object.h"

namespace Finch
{        
    double Value::AsNumber() const
    {
        ASSERT(mType == VALUE_NUMBER, "Value is not a number.");
        return mNumber;
    }
    
    String Value::AsString() const
    {
        ASSERT(mType == VALUE_STRING, "Value is not a string.");
        return mString;
    }
    
    Ref<Object> Value::AsObject() const
    {
        ASSERT(mType == VALUE_OBJECT, "Value is not an object.");
        return mObject;
    }
}