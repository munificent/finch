#include "Object.h"
#include "ArrayObject.h"
#include "BlockObject.h"
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
    
    Ref<Object> Object::NewNumber(Interpreter & interpreter, double value)
    {
        return Ref<Object>(new NumberObject(interpreter.NumberPrototype(), value));
    }
    
    Ref<Object> Object::NewString(Interpreter & interpreter, String value)
    {
        return Ref<Object>(new StringObject(interpreter.StringPrototype(), value));
    }
    
    Ref<Object> Object::NewArray(Interpreter & interpreter, int capacity)
    {
        return Ref<Object>(new ArrayObject(interpreter.ArrayPrototype(), capacity));
    }

    Ref<Object> Object::NewBlock(Interpreter & interpreter, Ref<Block> block,
                                 Ref<Object> self)
    {
        return Ref<Object>(new BlockObject(interpreter.BlockPrototype(), block, self));
    }
    
    Ref<Object> Object::NewFiber(Interpreter & interpreter, Ref<Object> block)
    {
        return Ref<Object>(new FiberObject(interpreter.FiberPrototype(),
                                           interpreter, block));
    }

    ostream & operator<<(ostream & cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}

