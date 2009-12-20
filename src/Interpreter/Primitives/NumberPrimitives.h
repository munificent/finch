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
    
    // Primitive methods for numbers.
    
    Ref<Object> NumberAdd(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberSubtract(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberMultiply(Ref<Object> thisRef, Environment & env,
                           String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberDivide(Ref<Object> thisRef, Environment & env,
                            String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberAbs(Ref<Object> thisRef, Environment & env,
                               String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberNeg(Ref<Object> thisRef, Environment & env,
                               String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberEquals(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberNotEquals(Ref<Object> thisRef, Environment & env,
                             String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberLessThan(Ref<Object> thisRef, Environment & env,
                                String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberGreaterThan(Ref<Object> thisRef, Environment & env,
                               String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberLessThanOrEqual(Ref<Object> thisRef, Environment & env,
                               String message, const vector<Ref<Object> > & args);
    
    Ref<Object> NumberGreaterThanOrEqual(Ref<Object> thisRef, Environment & env,
                                  String message, const vector<Ref<Object> > & args);
}