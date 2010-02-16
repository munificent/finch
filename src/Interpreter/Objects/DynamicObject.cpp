#include "DynamicObject.h"
#include "BlockObject.h"
#include "Environment.h"

namespace Finch
{
    using std::ostream;
    
    void DynamicObject::Trace(ostream & stream) const
    {
        stream << mName;
    }
    
    Ref<Object> DynamicObject::Receive(Ref<Object> thisRef, Environment & env, 
                                       String message, const vector<Ref<Object> > & args)
    {        
        // see if it's a method call
        Ref<Object> found = mMethods.Find(message);
        if (!found.IsNull())
        {
            BlockObject* block = found->AsBlock();
            
            ASSERT_NOT_NULL(block);
            
            Ref<Object> result = env.EvaluateMethod(thisRef, *block, args);
            
            return result;
        }
        
        // see if it's a primitive call
        map<String, PrimitiveMethod>::iterator primitive = mPrimitives.find(message);
        if (primitive != mPrimitives.end())
        {
            PrimitiveMethod method = primitive->second;
            
            ASSERT_NOT_NULL(method);
            
            return method(thisRef, env, message, args);
        }
        
        return Object::Receive(thisRef, env, message, args);
    }
    
    Ref<Object> DynamicObject::AddMethod(Environment & env, String name, Ref<Object> body)
    {
        if (name.Length() == 0)
        {
            env.RuntimeError("Cannot create a method with an empty name.");
            return Ref<Object>();
        }
        
        if (body->AsBlock() == NULL)
        {
            env.RuntimeError("Body of method must be a block.");
            return Ref<Object>();
        }
        
        // add the method
        mMethods.Insert(name, body);
        
        return Ref<Object>();        
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