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
                                String message, const vector<Ref<Object> > & args)
    {        
        // see if it's a method call
        Ref<Object> found = mMethods.Find(message);
        if (!found.IsNull())
        {
            BlockObject* block = found->AsBlock();
            
            ASSERT_NOT_NULL(block);
            
            interpreter.CallMethod(thisRef, *block, args);
            return;
        }
        
        // see if it's a primitive call
        map<String, PrimitiveMethod>::iterator primitive = mPrimitives.find(message);
        if (primitive != mPrimitives.end())
        {
            PrimitiveMethod method = primitive->second;
            ASSERT_NOT_NULL(method);
            
            method(thisRef, interpreter, message, args);
            return;
        }
        
        // if we got here, the message wasn't handled
        Object::Receive(thisRef, interpreter, message, args);
    }
    
    void DynamicObject::AddMethod(Environment & env, String name, Ref<Object> body)
    {
        if (name.Length() == 0)
        {
            env.RuntimeError("Cannot create a method with an empty name.");
            return;
        }
        
        if (body->AsBlock() == NULL)
        {
            env.RuntimeError("Body of method must be a block.");
            return;
        }
        
        // add the method
        mMethods.Insert(name, body);
    }
    
    void DynamicObject::RegisterPrimitive(String message, PrimitiveMethod method)
    {
        mPrimitives[message] = method;
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