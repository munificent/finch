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
    
    // Primitive methods for the global Ether object.
    Ref<Object> EtherQuit(Ref<Object> thisRef, Environment & env,
                                  String message, const vector<Ref<Object> > & args);
    
    // Flow control
    Ref<Object> EtherIfThen(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args);
    Ref<Object> EtherIfThenElse(Ref<Object> thisRef, Environment & env,
                              String message, const vector<Ref<Object> > & args);
    Ref<Object> EtherWhileDo(Ref<Object> thisRef, Environment & env,
                                   String message, const vector<Ref<Object> > & args);
    
    // Basic IO
    Ref<Object> EtherWrite(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
    
    Ref<Object> EtherWriteLine(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);

    Ref<Object> EtherLoad(Ref<Object> thisRef, Environment & env,
                                 String message, const vector<Ref<Object> > & args);
}