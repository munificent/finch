#include "Object.h"
#include "ArrayObject.h"
#include "BlockObject.h"
#include "DynamicObject.h"
#include "FiberObject.h"
#include "Interpreter.h"
#include "NumberObject.h"
#include "Fiber.h"
#include "StringObject.h"

namespace Finch
{
    using std::ostream;
    
    Value::Value(const Value & other)
    :   mObj(other.mObj)
    {
        if (mObj != NULL) mObj->mRefCount++;
    }
    
    const Value & Value::Parent() const { return mObj->Parent(); }

    Value & Value::operator =(const Value & other)
    {
        if (&other != this)
        {
            Clear();
            mObj = other.mObj;
            if (mObj != NULL) mObj->mRefCount++;
        }
        
        return *this;
    }
    
    void Value::Clear()
    {
        if (mObj != NULL)
        {
            mObj->mRefCount--;
            if (mObj->mRefCount == 0)
            {
                delete mObj;
            }
            
            mObj = NULL;
        }
    }
        
    double Value::AsNumber() const { return mObj->AsNumber(); }
    String Value::AsString() const { return mObj->AsString(); }
    ArrayObject * Value::AsArray() const { return mObj->AsArray(); }
    BlockObject * Value::AsBlock() const { return mObj->AsBlock(); }
    DynamicObject * Value::AsDynamic() const { return mObj->AsDynamic(); }
    FiberObject * Value::AsFiber() const { return mObj->AsFiber(); }
    
    ostream & operator<<(ostream & cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}

