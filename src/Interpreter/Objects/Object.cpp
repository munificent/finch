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
    
    double Value::AsNumber() const { return mObj->AsNumber(); }
    String Value::AsString() const { return mObj->AsString(); }
    ArrayObject * Value::AsArray() const { return mObj->AsArray(); }
    BlockObject * Value::AsBlock() const { return mObj->AsBlock(); }
    DynamicObject * Value::AsDynamic() const { return mObj->AsDynamic(); }
    FiberObject * Value::AsFiber() const { return mObj->AsFiber(); }
    
    ostream & operator<<(ostream & cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}

