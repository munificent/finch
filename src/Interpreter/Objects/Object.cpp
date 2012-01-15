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

    void Value::Trace(ostream & cout) const
    {
        if (IsNull())
        {
            cout << "(nil)";
        }
        else
        {
            mObj->Trace(cout);
        }
    }

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
    
    Value Value::GetField(int name) const
    {
        if (IsNull()) return Value();
        return mObj->GetField(name);
    }
    
    void Value::SetField(int name, const Value & value) const
    {
        if (IsNull()) return;
        return mObj->SetField(name, value);
    }

    Value Value::FindMethod(StringId name) const
    {
        if (IsNull()) return Value();
        return mObj->FindMethod(name);
    }

    PrimitiveMethod Value::FindPrimitive(StringId name) const
    {
        if (IsNull()) return NULL;
        return mObj->FindPrimitive(name);
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
    
    ostream & operator<<(ostream & cout, const Value & value)
    {
        value.Trace(cout);
        return cout;
    }
}

