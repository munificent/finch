#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // A simple stack of items with a fixed capacity. Implemented using a
    // single array. Push and pop are O(1). Stack items must support a default
    // constructor and copying.
    template <class T, int Size>
    class Stack
    {
    public:
        Stack()
        :   mCount(0)
        {}
        
        // Gets the number of items currently in the stack.
        int Count() const { return mCount; }
        
        // Gets whether or not the stack is empty.
        bool IsEmpty() const { return mCount == 0; }
        
        // Gets the maximum number of items the stack can hold.
        int Capacity() const { return Size; }
        
        // Pushes the given item onto the top of the stack.
        void Push(const T & item)
        {
            ASSERT(mCount < Capacity(), "Cannot push onto a full stack.");
            
            mItems[mCount++] = item;
        }
        
        // Pops the top item off the stack.
        T Pop()
        {
            ASSERT(mCount > 0, "Cannot pop an empty stack.");
            
            return mItems[--mCount];
        }
        
        // Gets the item at the given index in the stack. Index zero is the
        // top of the stack, and indices increase towards the bottom of the
        // stack.
        T & operator[] (int index)
        {
            ASSERT_INDEX(index, mCount);
            
            return mItems[mCount - 1 - index];
        }
        
    private:
        int mCount;
        T   mItems[Size];
        
        NO_COPY(Stack);
    };
}