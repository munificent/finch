#include "DynamicObject.h"
#include "BlockObject.h"
#include "Fiber.h"

namespace Finch
{
    using std::ostream;
    
    void DynamicObject::Trace(ostream & stream) const
    {
        stream << mName;
    }
    
    Value DynamicObject::FindMethod(StringId messageId)
    {
        Value method;
        if (mMethods.Find(messageId, &method))
        {
            return method;
        }
        
        return Value();
    }
    
    PrimitiveMethod DynamicObject::FindPrimitive(StringId messageId)
    {
        PrimitiveMethod primitive;
        if (mPrimitives.Find(messageId, &primitive))
        {
            return primitive;
        }
        
        return NULL;
    }
    
    Value DynamicObject::GetField(StringId name)
    {
        // Walk up the parent chain until it loops back on itself at Object.
        Object * object = this;
        while (true)
        {
            DynamicObject * dynamic = object->AsDynamic();
            
            // Only dynamic objects have fields, so skip others in the
            // inheritance chain.
            if (dynamic == NULL) continue;
            
            Value field;
            if (dynamic->mFields.Find(name, &field))
            {
                // Found it.
                return field;
            }

            // If we're at the root of the inheritance chain, then stop.
            if (&(*object->Parent()) == object) break;
            object = &(*object->Parent());
        }
        
        // If we get here, it wasn't found.
        return Value();
    }
    
    void DynamicObject::SetField(StringId name, const Value & value)
    {
        mFields.Insert(name, value);
    }
        
    void DynamicObject::AddMethod(StringId messageId, const Value & method)
    {
        mMethods.Insert(messageId, method);
    }

    void DynamicObject::AddPrimitive(StringId messageId, PrimitiveMethod method)
    {
        mPrimitives.Insert(messageId, method);
    }
}
