#pragma once

#include "Array.h"
#include "Macros.h"
#include "Ref.h"

namespace Finch
{
    class Object;

    class ArgReader
    {
    public:
        ArgReader(Array<Ref<Object> > & stack, int firstArg, int numArgs)
        :   mStack(stack),
            mFirstArg(firstArg),
            mNumArgs(numArgs)
        {}

        int StackStart() const { return mFirstArg; }
        int NumArgs() const { return mNumArgs; }

        Ref<Object> & operator[] (int index)
        {
            ASSERT_RANGE(index, mNumArgs);
            return mStack[mFirstArg + index];
        }

    private:
        int mFirstArg;
        int mNumArgs;
        Array<Ref<Object> > & mStack;
    };
}

