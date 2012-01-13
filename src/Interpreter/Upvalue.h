#pragma once

#include <fstream>
#include <iostream>

#include "Array.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

namespace Finch
{
    // TODO(bob): If we get rid of Ref<T> and use pointers and a more direct
    // value representation, this can be much simpler and we can get rid of
    // the weird passing in the stack thing.
    class Upvalue
    {
    public:
        // Default constructor so we can use it in Array<T>.
        Upvalue()
        :   mStackIndex(-1)
        {}
        
        Upvalue(int stackIndex)
        :   mStackIndex(stackIndex)
        {}
        
        Ref<Object> Get(Array<Ref<Object> > & stack) const;        
        void Set(Array<Ref<Object> > & stack, Ref<Object> value);        
        void Close(Array<Ref<Object> > & stack);        
        int Index() const;        
        bool IsOpen() const;

        Ref<Upvalue> Next() const { return mNext; }
        void SetNext(Ref<Upvalue> upvalue) { mNext = upvalue; }

    private:
        // TODO(bob): Can use a union for some of this.
        int mStackIndex;    // Will be -1 if Upvalue is closed.
        Ref<Object> mValue; // Only use when Upvalue is closed.
        Ref<Upvalue> mNext;
    };
}

