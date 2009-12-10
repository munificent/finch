#include "Object.h"

namespace Finch
{
    class NumberObject : public Object
    {
    public:
        NumberObject(double value)
        :   mValue(value)
        {}
        
        virtual void Trace(std::ostream & stream) const;
        
        virtual Ref<Object> Receive(String message, vector<Ref<Object> > args);

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
    
    void NumberObject::Trace(std::ostream & stream) const
    {
        stream << mValue;
    }

    Ref<Object> NumberObject::Receive(String message, vector<Ref<Object> > args)
    {
        if ((message == "abs") && (args.size() == 0))
        {
            double value = (mValue < 0) ? -mValue : mValue;
            return Object::New(value); 
        }
        else if ((message == "neg") && (args.size() == 0))
        {
            return Object::New(-mValue); 
        }
        
        //### bob: should do some sort of message not handled thing here
        return Ref<Object>();
    }
}