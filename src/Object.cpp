#include "Object.h"
#include "BlockObject.h"
#include "DynamicObject.h"
#include "NumberObject.h"

namespace Finch
{
    Ref<Object> Object::New()
    {
        return Ref<Object>(new DynamicObject());
    }
    
    Ref<Object> Object::New(double value)
    {
        return Ref<Object>(new NumberObject(value));
    }
    
    Ref<Object> Object::New(Ref<Scope> parentScope, Ref<Expr> value)
    {
        return Ref<Object>(new BlockObject(parentScope, value));
    }
    
    std::ostream& operator<<(std::ostream& cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}