#include <iostream>

#include "BooleanPrimitives.h"
#include "Environment.h"

namespace Finch
{
    Ref<Object> BooleanTrue(Ref<Object> thisRef, Environment & env,
                          String message, const vector<Ref<Object> > & args)
    {
        return env.True();
    }

    Ref<Object> BooleanFalse(Ref<Object> thisRef, Environment & env,
                            String message, const vector<Ref<Object> > & args)
    {
        return env.False();
    }
}
