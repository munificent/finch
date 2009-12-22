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
    
    // Primitive methods for booleans.
    
    Ref<Object> BooleanTrue(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args);
    
    Ref<Object> BooleanFalse(Ref<Object> thisRef, Environment & env,
                            String message, const vector<Ref<Object> > & args);
}