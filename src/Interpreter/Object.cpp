#include "Object.h"
#include "BlockObject.h"
#include "DynamicObject.h"
#include "NumberObject.h"
#include "StringObject.h"

namespace Finch
{
    Ref<Object> Object::New(Ref<Object> prototype, String name)
    {
        return Ref<Object>(new DynamicObject(prototype, name));
    }
    
    Ref<Object> Object::New(Ref<Object> prototype)
    {
        return Ref<Object>(new DynamicObject(prototype));
    }
    
    Ref<Object> Object::New(double value)
    {
        return Ref<Object>(new NumberObject(value));
    }
    
    Ref<Object> Object::New(String value)
    {
        return Ref<Object>(new StringObject(value));
    }
    
    Ref<Object> Object::New(Ref<Expr> value)
    {
        return Ref<Object>(new BlockObject(value));
    }
    
    std::ostream& operator<<(std::ostream& cout, const Object & object)
    {
        object.Trace(cout);
        return cout;
    }
}