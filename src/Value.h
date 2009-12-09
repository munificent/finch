#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class Object;
    
    enum ValueType
    {
        VALUE_NUMBER,
        VALUE_STRING,
        VALUE_OBJECT
    };
    
    class Value
    {
    public:
        Value(double number)
        :   mType(VALUE_NUMBER),
            mNumber(number),
            mString(),
            mObject()
        {}
        
        Value(String s)
        :   mType(VALUE_STRING),
            mNumber(0),
            mString(s),
            mObject()
        {}
        
        Value(Ref<Object> object)
        :   mType(VALUE_OBJECT),
            mNumber(0),
            mString(),
            mObject(object)
        {}
        
        ValueType Type() const { return mType; }
        
        double      AsNumber() const;
        String      AsString() const;
        Ref<Object> AsObject() const;
        
    private:
        ValueType   mType;
        
        double      mNumber;
        String      mString;
        Ref<Object> mObject;
    };
}