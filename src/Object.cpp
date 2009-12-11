#include "Object.h"

namespace Finch
{
    class NumberObject : public Object
    {
    public:
        NumberObject(double value)
        :   mValue(value)
        {}
        
        virtual void Trace(std::ostream & stream) const { stream << mValue; }
        
        virtual Ref<Object> Receive(String message, vector<Ref<Object> > args);
        
        virtual double AsNumber() const { return mValue; }

    private:
        double mValue;
    };
    
    Ref<Object> Object::New(double value)
    {
        return Ref<Object>(new NumberObject(value));
    }
    
    std::ostream& operator<<(std::ostream& cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
    
    Ref<Object> NumberObject::Receive(String message, vector<Ref<Object> > args)
    {
        //### bob: this could be refactored into something more maintainable
        if ((message == "abs") && (args.size() == 0))
        {
            double value = (mValue < 0) ? -mValue : mValue;
            return Object::New(value); 
        }
        else if ((message == "neg") && (args.size() == 0))
        {
            return Object::New(-mValue); 
        }
        else if ((message == "+") && (args.size() == 1))
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue + value); 
        }
        else if ((message == "-") && (args.size() == 1))
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue - value); 
        }
        else if ((message == "*") && (args.size() == 1))
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue * value); 
        }
        else if ((message == "/") && (args.size() == 1))
        {
            double value = args[0]->AsNumber();
            return Object::New(mValue / value); 
        }
        //### bob: temp!
        else if ((message == "hack:temp:") && (args.size() == 2))
        {
            double value1 = args[0]->AsNumber();
            double value2 = args[1]->AsNumber();
            return Object::New(mValue + value1 + value2); 
        }
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}