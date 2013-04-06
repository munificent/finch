#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // A simple queue of items. Implemented using a dynamically sized circular
    // buffer. Enqueue and dequeue are O(1). Queue items must support a default
    // constructor and copying.
    template <class T>
    class Queue
    {
    public:
        Queue()
        :   mHead(0),
            mCount(0),
            mCapacity(0),
            mItems(NULL)
        {}

        ~Queue()
        {
            Clear();
        }
        
        // Gets the number of items currently in the queue.
        int Count() const { return mCount; }
        
        // Gets whether or not the queue is empty.
        bool IsEmpty() const { return mCount == 0; }
        
        // Clears the entire queue.
        void Clear()
        {
            while (!IsEmpty()) Dequeue();
        }
        
        // Adds the given item to the head of the queue.
        void Enqueue(const T & item)
        {
            EnsureCapacity(mCount + 1);
            
            mCount++;
            mItems[mHead] = item;
            mHead = Wrap(mHead + 1);
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
        int Wrap(int index) const { return (index + mCapacity) % mCapacity; }

        void EnsureCapacity(int desiredCapacity)
        {
            // early out if we have enough capacity
            if (mCapacity >= desiredCapacity) return;

            // figure out the new array size
            // instead of growing to just the capacity we need, we'll grow by
            // a multiple of the current size. this ensures amortized O(n)
            // complexity on adding instead of O(n^2).
            int capacity = mCapacity;
            if (capacity < MIN_CAPACITY) capacity = MIN_CAPACITY;

            while (capacity < desiredCapacity)
            {
                capacity *= GROW_FACTOR;
            }

            // create the new array
            T* newItems = new T[capacity];

            // copy the items over
            for (int i = 0; i < mCount; i++)
            {
                newItems[i] = mItems[Wrap(mHead - mCount + i)];
            }

            // delete the old one
            delete [] mItems;
            mHead = mCount;
            mItems = newItems;
            mCapacity = capacity;
        }

        static const int MIN_CAPACITY = 4;
        static const int GROW_FACTOR  = 2;

        int mHead;
        int mCount;
        int mCapacity;
        T*  mItems;

        NO_COPY(Queue);
    };
}

