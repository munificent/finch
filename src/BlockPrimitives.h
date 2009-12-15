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
    
    Ref<Object> BlockValue(Ref<Object> thisRef, EvalContext & context,
                           String message, const vector<Ref<Object> > & args);
    
    Ref<Object> BlockWhile(Ref<Object> thisRef, EvalContext & context,
                             String message, const vector<Ref<Object> > & args);
}