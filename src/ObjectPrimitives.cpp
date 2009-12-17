#include <iostream>

#include "ObjectPrimitives.h"
#include "DynamicObject.h"
#include "Environment.h"
#include "Object.h"

namespace Finch
{
    Ref<Object> ObjectCopy(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        return Object::NewObject(thisRef);
    }

    Ref<Object> ObjectAddFieldValue(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        DynamicObject* object = thisRef->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        return object->AddField(name, value);
    }

    Ref<Object> ObjectAddMethodValue(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args)
    {
        DynamicObject* object = thisRef->AsDynamic();
        ASSERT_NOT_NULL(object);
        
        String      name  = args[0]->AsString();
        Ref<Object> value = args[1];
        
        return object->AddMethod(name, value);
    }
}
