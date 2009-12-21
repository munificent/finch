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
    
    // Primitive methods for blocks.
    
    Ref<Object> BlockCall(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
}