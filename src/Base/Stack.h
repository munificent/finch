#pragma once

#include <iostream>

#include "Array.h"
#include "Macros.h"

namespace Finch
{
    // A simple stack of items with a variable capacity. Implemented using a
    // dynamic array. Push and pop are O(1) (amortized). Stack items must
    // support a default constructor and copying.
    template <class T>
    class Stack
    {
    public:
        Stack()
        :   mItems()
        {}
        
        // Gets the number of items currently in the stack.
        int Count() const { return mItems.Count(); }
        
        // Gets whether or not the stack is empty.
        bool IsEmpty() const { return mItems.IsEmpty(); }
        
        // Pushes the given item onto the top of the stack.
        void Push(const T & item)
        {
            mItems.Add(item);
        }
        
        // Pops the top item off the stack.
        T Pop()
        {
            ASSERT(!IsEmpty(), "Cannot pop an empty stack.");
            
            T popped = mItems[-1];
            mItems.Remove(-1);
            
            return popped;
        }
        
        // Returns the item on the top of the stack without removing it.
        T & Peek()
        {
            ASSERT(!IsEmpty(), "Cannot peek an empty stack.");
            
            return mItems[-1];
        }
        
        // Gets the item at the given index in the stack. Index zero is the
        // top of the stack, and indices increase towards the bottom of the
        // stack.
        T & operator[] (int index)
        {
            ASSERT_INDEX(index, Count());
            
            return mItems[-1 - index];
        }
        
    private:
        Array<T> mItems;
        
        NO_COPY(Stack);
    };
}

