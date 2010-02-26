#include "DynamicObject.h"
#include "BlockObject.h"
#include "Environment.h"
#include "Interpreter.h"

namespace Finch
{
    using std::ostream;
    
    void DynamicObject::Trace(ostream & stream) const
    {
        stream << mName;
    }
    
    void DynamicObject::Receive(Ref<Object> thisRef, Interpreter & interpreter, 
                                String message, const Array<Ref<Object> > & args)
    {        
        // see if it's a method call
        Ref<Object> method;
        if (mMethods.Find(message, &method))
        {
            BlockObject* block = method->AsBlock();
            
            ASSERT_NOT_NULL(block);
            
            interpreter.CallMethod(thisRef, *block, args);
            return;
        }
        
        // see if it's a primitive call
        PrimitiveMethod primitive;
        if (mPrimitives.Find(message, &primitive))
        {
            ASSERT_NOT_NULL(primitive);
            
            primitive(thisRef, interpreter, message, args);
            return;
        }
        
        // if we got here, the message wasn't handled
        Object::Receive(thisRef, interpreter, message, args);
    }
    
    void DynamicObject::AddMethod(Interpreter & interpreter, String name, Ref<Object> body)
    {
        if (name.Length() == 0)
        {
            interpreter.RuntimeError("Cannot create a method with an empty name.");
            return;
        }
        
        if (body->AsBlock() == NULL)
        {
            interpreter.RuntimeError("Body of method must be a block.");
            return;
        }
        
        // add the method
        mMethods.Insert(name, body);
    }
    
    void DynamicObject::RegisterPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives.Insert(message, method);
    }

    void DynamicObject::InitializeScope()
    {
        if (!Prototype().IsNull())
        {
            mScope = Ref<Scope>(new Scope(Prototype()->ObjectScope()));
        }
        else
        {
            mScope = Ref<Scope>(new Scope());
        }
    }
}