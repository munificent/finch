#include "Object.h"
#include "ArrayObject.h"
#include "BlockObject.h"
#include "Environment.h"
#include "DynamicObject.h"
#include "FiberObject.h"
#include "Interpreter.h"
#include "NumberObject.h"
#include "Fiber.h"
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
    
    Ref<Object> Object::NewArray(Environment & env, int capacity)
    {
        return Ref<Object>(new ArrayObject(env.ArrayPrototype(), capacity));
    }

    Ref<Object> Object::NewBlock(Environment & env, Ref<Block> block,
                                 Ref<Object> self)
    {
        return Ref<Object>(new BlockObject(env.BlockPrototype(), block, self));
    }
    
    Ref<Object> Object::NewFiber(Interpreter & interpreter, Ref<Object> block)
    {
        return Ref<Object>(new FiberObject(interpreter.GetEnvironment().FiberPrototype(),
                                           interpreter, block));
    }

    ostream & operator<<(ostream & cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}

