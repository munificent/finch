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
    
    void DynamicObject::AddMethod(Ref<Object> self, Fiber & fiber,
                                  String name, Ref<Scope> closure,
                                  Ref<CodeBlock> code)
    {
        if (name.Length() == 0)
        {
            fiber.Error("Cannot create a method with an empty name.");
            return;
        }
        Ref<Object> body = Object::NewBlock(fiber.GetEnvironment(), code,
                                            closure, self);
        
        // add the method
        mMethods.Insert(name, body);
    }
    
    void DynamicObject::AddPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives.Insert(message, method);
    }
     */
}
