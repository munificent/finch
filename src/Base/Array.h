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
            mCapacity(0),
            mItems(NULL)
        {
            EnsureCapacity(capacity);
        }
        
        Array(const Array<T> & array)
        :   mCount(0),
            mCapacity(0),
            mItems(NULL)
        {
            Add(array);
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
            EnsureCapacity(mCount + 1);
            
            mItems[mCount++] = value;
        }
        
        // Adds all of the items from the given array to this one.
        void Add(const Array<T> & array)
        {
            EnsureCapacity(mCount + array.mCount);
            
            for (int i = 0; i < array.mCount; i++)
            {
                mItems[mCount++] = array[i];
            }
        }
        
        // Removes the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        void Remove(int index)
        {
            if (index < 0) index = mCount + index;
            ASSERT_INDEX(index, mCount);
            
            // shift items up
            for (int i = index; i < mCount - 1; i++)
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
        
        // Reverses the order of the items in the array.
        void Reverse()
        {
            for (int i = 0; i < mCount / 2; i++)
            {
                T temp = mItems[i];
                mItems[i] = mItems[mCount - i - 1];
                mItems[mCount - i - 1] = temp;
            }
        }
        
    private:
        void EnsureCapacity(int desiredCapacity)
        {
            // early out if we have enough capacity
            if (mCapacity >= desiredCapacity) return;
            
            // figure out the new array size
            // instead of growing to just the capacity we need, we'll grow by
            // a multiple of the current size. this ensures amortized O(n)
            // complexity on adding instead of O(n^2).
            int capacity = mCapacity;
            if (capacity < MIN_CAPACITY)
            {
                capacity = MIN_CAPACITY;
            }

            while (capacity < desiredCapacity)
            {
                capacity *= GROW_FACTOR;
            }
        
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
    };
}