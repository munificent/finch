#pragma once

#include "Array.h"
#include "Macros.h"
#include "Ref.h"

namespace Finch
{
    class Object;
    class Value;

    class ArgReader
    {
    public:
        ArgReader(Array<Value> & stack, int firstArg, int numArgs)
        :   mStack(stack),
            mFirstArg(firstArg),
            mNumArgs(numArgs)
        {}

        int StackStart() const { return mFirstArg; }
        int NumArgs() const { return mNumArgs; }

        const Value & operator[] (int index) const
        {
            ASSERT_RANGE(index, mNumArgs);
            return mStack[mFirstArg + index];
        }

    private:
        Array<Value> & mStack;
        int mFirstArg;
        int mNumArgs;
    };
}

