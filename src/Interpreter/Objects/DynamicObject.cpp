#include "DynamicObject.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Fiber.h"

namespace Finch
{
    using std::ostream;
    
    void DynamicObject::Trace(ostream & stream) const
    {
        stream << mName;
    }
    
    Ref<Object> DynamicObject::FindMethod(int messageId)
    {
        Ref<Object> method;
        if (mMethods.Find(messageId, &method))
        {
            return method;
        }
        
        return Ref<Object>();
    }
    
    PrimitiveMethod DynamicObject::FindPrimitive(int messageId)
    {
        PrimitiveMethod primitive;
        if (mPrimitives.Find(messageId, &primitive))
        {
            return primitive;
        }
        
        return NULL;
    }
    
    Ref<Object> DynamicObject::GetField(int name)
    {
        // Walk up the parent chain until it loops back on itself at Object.
        Object & object = *this;
        while (true)
        {
            DynamicObject * dynamic = object.AsDynamic();
            
            // Only dynamic objects have fields, so skip others in the
            // inheritance chain.
            if (dynamic == NULL) continue;
            
            Ref<Object> field;
            if (dynamic->mFields.Find(name, &field))
            {
                // Found it.
                return field;
            }

            // If we're at the root of the inheritance chain, then stop.
            if (&(*object.Parent()) == &object) break;

            // Walk up the parent chain.
            object = *object.Parent();
        }
        
        // If we get here, it wasn't found.
        return Ref<Object>();
    }
    
    void DynamicObject::SetField(int name, Ref<Object> value)
    {
        mFields.Insert(name, value);
    }
    
    /*
    void DynamicObject::Receive(Ref<Object> self, Fiber & fiber, 
                                String message, const Array<Ref<Object> > & args)
    {        
        // see if it's a method call
        Ref<Object> method;
        if (mMethods.Find(message, &method))
        {
            ASSERT_NOT_NULL(method->AsBlock());
            fiber.CallMethod(self, method, args);
            return;
        }
        
        // see if it's a primitive call
        PrimitiveMethod primitive;
        if (mPrimitives.Find(message, &primitive))
        {
            ASSERT_NOT_NULL(primitive);
            
            primitive(self, fiber, message, args);
            return;
        }
        
        // if we got here, the message wasn't handled
        Object::Receive(self, fiber, message, args);
    }
    */
    
    void DynamicObject::AddMethod(int messageId, Ref<Object> method)
    {
        mMethods.Insert(messageId, method);
    }

    void DynamicObject::AddPrimitive(int messageId, PrimitiveMethod method)
    {
        mPrimitives.Insert(messageId, method);
    }
}
