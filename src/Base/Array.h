#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // A resizable dynamic array class. Array items must support copying and
    // a default constructor.
    template <class T>
    class Array
    {
    public:
        Array()
        :   mCount(0),
            mCapacity(0),
            mItems(NULL)
        {}
        
        Array(int capacity)
        :   mCount(0),
            mCapacity(capacity),
            mItems(NULL)
        {
            Resize(mCapacity);
        }
        
        ~Array()
        {
            if (mItems != NULL) delete [] mItems;
        }
        
        // Gets the number of items currently in the array.
        int Count() const { return mCount; }
        
        // Gets whether or not the array is empty.
        bool IsEmpty() const { return mCount == 0; }
        
        // Gets the maximum number of array the stack can hold before a
        // reallocation will occur.
        int Capacity() const { return mCapacity; }
        
        // Adds the given item to the end of the array, increasing its size
        // automatically.
        void Add(const T & value)
        {
            // grow the array if needed
            if (mCount >= mCapacity)
            {
                if (mCapacity < MIN_CAPACITY)
                {
                    Resize(MIN_CAPACITY);
                }
                else
                {
                    Resize(mCapacity * GROW_FACTOR);
                }

            }
            
            mItems[mCount++] = value;
        }
        
        // Removes the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        void Remove(int index)
        {
            if (index < 0) index = mCount + index;
            ASSERT_INDEX(index, mCount);
            
            // shift items up
            for (int i = index; i < mCount; i++)
            {
                mItems[i] = mItems[i + 1];
            }
            
            mItems[mCount - 1] = T(); // clear the copy of the last item
            mCount--;
        }
        
        // Gets the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        T & operator[] (int index)
        {
            if (index < 0) index = mCount + index;
            ASSERT_INDEX(index, mCount);
            
            return mItems[index];
        }
        
        // Gets the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        const T & operator[] (int index) const
        {
            if (index < 0) index = mCount + index;
            ASSERT_INDEX(index, mCount);
            
            return mItems[index];
        }
        
    private:
        void Resize(int capacity)
        {
            ASSERT(capacity >= mCount, "Resize cannot truncate items.");
            
            // create the new array
            T* newItems = new T[capacity];
            
            // copy the items over
            for (int i = 0; i < mCount; i++)
            {
                newItems[i] = mItems[i];
            }
            
            // delete the old one
            delete [] mItems;
            mItems = newItems;
            
            mCapacity = capacity;
        }
        
        static const int MIN_CAPACITY = 16;
        static const int GROW_FACTOR  = 2;
        
        int mCount;
        int mCapacity;
        T*  mItems;
        
        NO_COPY(Array);
    };
}