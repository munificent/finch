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
    
    // Primitive methods for the global Console object.
    
    Ref<Object> ConsoleWrite(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
    
    Ref<Object> ConsoleWriteLine(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
}