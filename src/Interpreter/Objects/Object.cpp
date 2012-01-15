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
    
    const Value & Value::Parent() const { return mObj->Parent(); }

    void Value::Clear()
    {
        if (mNext != this)
        {
            // unlink it
            mPrev->mNext = mNext;
            mNext->mPrev = mPrev;
            
            mPrev = this;
            mNext = this;
        }
        else if (mObj != NULL)
        {
            // linked to itself, so it's the last reference
            delete mObj;
        }
        
        mObj = NULL;
    }
    
    void Value::Link(const Value & other)
    {
        // don't bother to share null
        if (other.mObj != NULL)
        {
            mObj = other.mObj;
            
            // link it in
            mPrev = other.mPrev;
            mNext = &other;
            
            other.mPrev = this;
            mPrev->mNext = this;
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

