#include "Object.h"
#include "ArrayObject.h"
#include "BlockObject.h"
#include "Environment.h"
#include "DynamicObject.h"
#include "FiberObject.h"
#include "Interpreter.h"
#include "NumberObject.h"
#include "Process.h"
#include "Scope.h"
#include "StringObject.h"

namespace Finch
{
    using std::ostream;
    
    Ref<Object> Object::NewObject(Ref<Object> parent, String name)
    {
        Ref<Object> object = Ref<Object>(new DynamicObject(parent, name));
        
        // if the object has no parent, use itself as it
        if (parent.IsNull())
        {
            object->SetParent(object);
        }
        
        return object;
    }
    
    Ref<Object> Object::NewObject(Ref<Object> parent)
    {
        return NewObject(parent, "");
    }
    
    Ref<Object> Object::NewNumber(Environment & env, double value)
    {
        return Ref<Object>(new NumberObject(env.NumberPrototype(), value));
    }
    
    Ref<Object> Object::NewString(Environment & env, String value)
    {
        return Ref<Object>(new StringObject(env.StringPrototype(), value));
    }
    
    Ref<Object> Object::NewArray(Environment & env, int length)
    {
        return Ref<Object>(new ArrayObject(env, env.ArrayPrototype(), length));
    }

    Ref<Object> Object::NewBlock(Environment & env, const CodeBlock & code,
                                 Ref<Scope> closure, Ref<Object> self)
    {
        return Ref<Object>(new BlockObject(env.BlockPrototype(), code, closure, self));
    }
    
    Ref<Object> Object::NewFiber(Interpreter & interpreter, Ref<Object> block)
    {
        return Ref<Object>(new FiberObject(interpreter.GetEnvironment().FiberPrototype(),
                                           interpreter, block));
    }

    void Object::Receive(Ref<Object> self, Process & process,
                                String message, const Array<Ref<Object> > & args)
    {
        // walk up the parent chain until it loops back on itself at
        // Object: the object from whence all others are born
        if (&(*mParent) != this)
        {
            // we're using thisRef and not the parent's own this reference
            // on purpose. this way, if you send a "copy" message to some
            // object a few links down the parent chain from Object, you'll
            // get a copy of *that* object, and not Object itself where "copy"
            // is implemented.
            mParent->Receive(self, process, message, args);
        }
        else
        {
            //### bob: should do some sort of message not handled thing here
            String error = String::Format("Object '%s' did not handle message '%s'",
                                          self->AsString().CString(),
                                          message.CString());
            process.Error(error);
            process.PushNil();
        }
    }
    
    Ref<Scope> Object::ObjectScope() const
    {
        return Ref<Scope>();
    }

    ostream & operator<<(ostream & cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}

