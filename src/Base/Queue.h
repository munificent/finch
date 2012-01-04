#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // A simple queue of items with a fixed capacity. Implemented using a
    // circular buffer. Enqueue and dequeue are O(1). Queue items must support
    // a default constructor and copying.
    template <class T, int Size>
    class Queue
    {
    public:
        Queue()
        :   mHead(0),
            mCount(0)
        {}
        
        // Gets the number of items currently in the queue.
        int Count() const { return mCount; }
        
        // Gets whether or not the queue is empty.
        bool IsEmpty() const { return mCount == 0; }
        
        // Gets the maximum number of items the queue can hold.
        int Capacity() const { return Size; }
        
        // Clears the entire queue.
        void Clear()
        {
            while (!IsEmpty()) Dequeue();
        }
        
        // Adds the given item to the head of the queue.
        void Enqueue(const T & item)
        {
            ASSERT(mCount < Capacity(), "Cannot enqueue a full queue.");
            
            mItems[mHead] = item;
            mHead = Wrap(mHead + 1);
            mCount++;
        }
        
        // Removes the tail item from the queue.
        T Dequeue()
        {
            ASSERT(mCount > 0, "Cannot dequeue an empty queue.");
            
            int tail = Wrap(mHead - mCount);
            
            // clear the item from the queue
            T dequeued = mItems[tail];
            mItems[tail] = T();
            
            mCount--;
            return dequeued;
        }
        
        // Gets the item at the given index in the queue. Index zero is the
        // next item which will be dequeued. Index one is the item after that,
        // etc.
        T & operator[] (int index)
        {
            ASSERT_RANGE(index, mCount);
            
            return mItems[Wrap(mHead - mCount + index)];
        }
        
    private:
        int Wrap(int index) const { return (index + Size) % Size; }
        
        int mHead;
        int mCount;
        T   mItems[Size];
        
        NO_COPY(Queue);
    };
}

