#include "Value.h"
#include "Object.h"

namespace Finch
{
    //### bob: not used anymore. everything's an object!
    /*
    // "hidden" subclasses
    class NumberValue : public Value
    {
        friend class Value;
        
    public:
        virtual double AsNumber() const { return mNumber; }
        
    private:
        NumberValue(double number)
        :   Value(VALUE_NUMBER),
            mNumber(number)
        {}
        
        double      mNumber;
    };

    class StringValue : public Value
    {
        friend class Value;
        
    public:
        virtual String AsString() const { return mString; }
        
    private:
        StringValue(String s)
        :   Value(VALUE_STRING),
            mString(s)
        {}
        
        String      mString;
    };

    class ObjectValue : public Value
    {
        friend class Value;
        
    public:
        virtual Ref<Object> AsObject() const { return mObject; }
        
    private:
        ObjectValue(Ref<Object> object)
        :   Value(VALUE_OBJECT),
            mObject(object)
        {}
        
        Ref<Object> mObject;
    };
    
    // virtual constructors
    Ref<Value> Value::New(double value)
    {
        return Ref<Value>(new NumberValue(value));
    }
    
    Ref<Value> Value::New(String value)
    {
        return Ref<Value>(new StringValue(value));
    }
    
    Ref<Value> Value::New(Ref<Object> value)
    {
        return Ref<Value>(new ObjectValue(value));
    }
    
    // default downcast functions
    double Value::AsNumber() const
    {
        ASSERT(false, "Value is not a number.");
        return 0;
    }
    
    String Value::AsString() const
    {
        ASSERT(false, "Value is not a string.");
        return "";
    }
    
    Ref<Object> Value::AsObject() const
    {
        ASSERT(false, "Value is not an object.");
        return Ref<Object>();
    }
    */
}