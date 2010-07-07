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
    
    void DynamicObject::Receive(Ref<Object> thisRef, Process & process, 
                                String message, const Array<Ref<Object> > & args)
    {        
        // see if it's a method call
        Ref<Object> method;
        if (mMethods.Find(message, &method))
        {
            ASSERT_NOT_NULL(method->AsBlock());
            
            process.CallMethod(thisRef, method, args);
            return;
        }
        
        // see if it's a primitive call
        PrimitiveMethod primitive;
        if (mPrimitives.Find(message, &primitive))
        {
            ASSERT_NOT_NULL(primitive);
            
            primitive(thisRef, process, message, args);
            return;
        }
        
        // if we got here, the message wasn't handled
        Object::Receive(thisRef, process, message, args);
    }
    
    void DynamicObject::AddMethod(Ref<Object> thisRef, Process & process,
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
        
        // copy the block since rebinding self mutates it
        Ref<Object> blockCopy = Object::NewBlock(process.GetEnvironment(),
                                                 originalBlock->GetCode(),
                                                 originalBlock->Closure(),
                                                 originalBlock->Self());
        BlockObject * block = blockCopy->AsBlock();

        // rebind the block's self to this object
        block->RebindSelf(thisRef);
        
        // add the method
        mMethods.Insert(name, blockCopy);
    }
    
    void DynamicObject::RegisterPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives.Insert(message, method);
    }

    void DynamicObject::InitializeScope()
    {
        if (!Parent().IsNull())
        {
            mScope = Ref<Scope>(new Scope(Parent()->ObjectScope()));
        }
        else
        {
            mScope = Ref<Scope>(new Scope());
        }
    }
}

