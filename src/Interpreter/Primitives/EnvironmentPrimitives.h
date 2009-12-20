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
    
    // Primitive methods for the global Environment object.
    Ref<Object> EnvironmentQuit(Ref<Object> thisRef, Environment & env,
                                  String message, const vector<Ref<Object> > & args);
    
    // Flow control
    Ref<Object> EnvironmentIfThen(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args);
    Ref<Object> EnvironmentIfThenElse(Ref<Object> thisRef, Environment & env,
                              String message, const vector<Ref<Object> > & args);
    Ref<Object> EnvironmentWhileDo(Ref<Object> thisRef, Environment & env,
                                   String message, const vector<Ref<Object> > & args);
    
    // Basic IO
    Ref<Object> EnvironmentWrite(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
    
    Ref<Object> EnvironmentWriteLine(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);

    Ref<Object> EnvironmentLoad(Ref<Object> thisRef, Environment & env,
                                 String message, const vector<Ref<Object> > & args);
}