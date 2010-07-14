#include "DynamicObject.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Process.h"

namespace Finch
{
    using std::ostream;
    
    void DynamicObject::Trace(ostream & stream) const
    {
        stream << mName;
    }
    
    void DynamicObject::Receive(Ref<Object> self, Process & process, 
                                String message, const Array<Ref<Object> > & args)
    {        
        // see if it's a method call
        Ref<Object> method;
        if (mMethods.Find(message, &method))
        {
            ASSERT_NOT_NULL(method->AsBlock());
            
            process.CallMethod(self, method, args);
            return;
        }
        
        // see if it's a primitive call
        PrimitiveMethod primitive;
        if (mPrimitives.Find(message, &primitive))
        {
            ASSERT_NOT_NULL(primitive);
            
            primitive(self, process, message, args);
            return;
        }
        
        // if we got here, the message wasn't handled
        Object::Receive(self, process, message, args);
    }
    
    void DynamicObject::AddMethod(Ref<Object> self, Process & process,
                                  String name, Ref<Object> body)
    {
        if (name.Length() == 0)
        {
            process.Error("Cannot create a method with an empty name.");
            return;
        }
        
        BlockObject * originalBlock = body->AsBlock();
        if (originalBlock == NULL)
        {
            process.Error("Body of method must be a block.");
            return;
        }
        
        // make a copy of the block bound to our self
        Ref<Object> blockCopy = Object::NewBlock(process.GetEnvironment(),
                                                 originalBlock->GetCode(),
                                                 originalBlock->Closure(), self);
        
        // add the method
        mMethods.Insert(name, blockCopy);
    }
    
    void DynamicObject::RegisterPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives.Insert(message, method);
    }
}

