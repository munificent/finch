#pragma once

#include <iostream>

#include "Macros.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    //### bob: not used anymore. everything's an object!
    /*
    class Object;
    
    enum ValueType
    {
        VALUE_NUMBER,
        VALUE_STRING,
        VALUE_OBJECT
    };
    
    //### bob: instead of having all possible fields in one class, should use
    // subclasses and virtual methods like Token/StringToken/NumberToken.
    class Value
    {
    public:
        // virtual constructors
        static Ref<Value> New(double value);
        static Ref<Value> New(String value);
        static Ref<Value> New(Ref<Object> value);
        
        virtual ~Value() {}
        
        ValueType Type() const { return mType; }
        
        virtual double      AsNumber() const;
        virtual String      AsString() const;
        virtual Ref<Object> AsObject() const;
        
    protected:
        Value(ValueType type)
        :   mType(type)
        {}
        
    private:
        ValueType   mType;
    };
    */
}