#pragma once

#include <vector>

#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    // Primitive methods for dynamic (i.e. regular) objects.
    
    Ref<Object> ObjectSelf(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    Ref<Object> ObjectEquals(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    Ref<Object> ObjectNotEquals(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    Ref<Object> ObjectCopy(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    Ref<Object> ObjectAddMethodValue(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
}