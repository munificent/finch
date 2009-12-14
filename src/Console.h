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
    
    Ref<Object> ConsoleWrite(Ref<Object> thisRef, EvalContext & context,
                             String message, vector<Ref<Object> > args);
    
    Ref<Object> ConsoleWriteLine(Ref<Object> thisRef, EvalContext & context,
                             String message, vector<Ref<Object> > args);
}