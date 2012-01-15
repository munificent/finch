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
    
    Ref<Object> DynamicObject::FindMethod(StringId messageId)
    {
        Ref<Object> method;
        if (mMethods.Find(messageId, &method))
        {
            return method;
        }
        
        return Ref<Object>();
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
    
    Ref<Object> DynamicObject::GetField(StringId name)
    {
        // Walk up the parent chain until it loops back on itself at Object.
        Object * object = this;
        while (true)
        {
            DynamicObject * dynamic = object->AsDynamic();
            
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
            if (&(*object->Parent().Obj()) == object) break;
            object = &(*object->Parent().Obj());
        }
        
        // If we get here, it wasn't found.
        return Ref<Object>();
    }
    
    void DynamicObject::SetField(StringId name, Ref<Object> value)
    {
        mFields.Insert(name, value);
    }
        
    void DynamicObject::AddMethod(StringId messageId, Ref<Object> method)
    {
        mMethods.Insert(messageId, method);
    }

    void DynamicObject::AddPrimitive(StringId messageId, PrimitiveMethod method)
    {
        mPrimitives.Insert(messageId, method);
    }
}
