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
        // Only dynamic objects have fields.
        DynamicObject * dynamic = AsDynamic();
        if (dynamic == NULL) return Value();
        
        return dynamic->GetField(name);
    }
    
    void Value::SetField(int name, const Value & value) const
    {
        // Only dynamic objects have fields.
        DynamicObject * dynamic = AsDynamic();
        if (dynamic == NULL) return;
        
        dynamic->SetField(name, value);
    }

    Value Value::SendMessage(Fiber & fiber, StringId messageId, const ArgReader & args) const
    {
        const Value * receiver = this;
        
        // Walk the parent chain looking for a method that matches the message.
        while (true)
        {
            // Only dynamic objects have methods.
            DynamicObject * dynamic = receiver->AsDynamic();
            if (dynamic != NULL)
            {
                // See if the object has a method bound to that name.
                Value method = dynamic->FindMethod(messageId);
                if (!method.IsNull())
                {
                    fiber.CallBlock(*this, method, args);
                    return Value();
                }
                
                // See if the object has a primitive bound to that name.
                PrimitiveMethod primitive = dynamic->FindPrimitive(messageId);
                if (primitive != NULL)
                {
                    return primitive(fiber, *this, args);
                }
            }
            
            // If we're at the root of the inheritance chain, then stop.
            if (receiver->Parent().IsNull()) break;
            receiver = &receiver->Parent();
        }
        
        // If we got here, the object didn't handle the message.
        String messageName = fiber.GetInterpreter().FindString(messageId);
        String error = String::Format("Object '%s' did not handle message '%s'",
                                      AsString().CString(), messageName.CString());
        fiber.Error(error);
        
        // Unhandled messages just return nil.
        return fiber.Nil();
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

