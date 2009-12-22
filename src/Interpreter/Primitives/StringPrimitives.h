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
    
    // Primitive methods for string objects.
    
    Ref<Object> StringAdd(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    
    Ref<Object> StringLength(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args);
    
    Ref<Object> StringAt(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
    
    Ref<Object> StringEquals(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args);
    
    Ref<Object> StringNotEquals(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
}