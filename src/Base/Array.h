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

        Array(int size, const T & fillWith)
        :   mCount(0),
            mCapacity(0),
            mItems(NULL)
        {
            EnsureCapacity(size);
            
            for (int i = 0; i < size; i++)
            {
                mItems[i] = fillWith;
            }
            
            mCount = size;
        }
        
        Array(const Array<T> & array)
        :   mCount(0),
            mCapacity(0),
            mItems(NULL)
        {
            AddAll(array);
        }
        
        ~Array()
        {
            Clear();
        }
        
        // Gets the number of items currently in the array.
        int Count() const { return mCount; }
        
        // Gets whether or not the array is empty.
        bool IsEmpty() const { return mCount == 0; }
        
        // Gets the index of the given item in the array or -1 if not found.
        int IndexOf(const T & other) const
        {
            for (int i = 0; i < mCount; i++)
            {
                if (mItems[i] == other) return i;
            }
            return -1;
        }
        
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
        void AddAll(const Array<T> & array)
        {
            EnsureCapacity(mCount + array.mCount);
            
            for (int i = 0; i < array.mCount; i++)
            {
                mItems[mCount++] = array[i];
            }
        }
        
        // Removes all items from the array.
        void Clear()
        {
            if (mItems != NULL)
            {
                delete [] mItems;
                mItems = NULL;
            }
            mCount = 0;
            mCapacity = 0;
        }
        
        // Removes the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        void RemoveAt(int index)
        {
            if (index < 0) index = mCount + index;
            ASSERT_RANGE(index, mCount);
            
            // Shift items up.
            for (int i = index; i < mCount - 1; i++)
            {
                mItems[i] = mItems[i + 1];
            }
            
            mItems[mCount - 1] = T(); // clear the copy of the last item
            mCount--;
        }
        
        // Truncates the array to `count` or fewer items by removing trailing
        // items.
        void Truncate(int count)
        {
            ASSERT(count >= 0, "Cannot truncate to a negative count.");
            
            // Early out if there's nothing to remove.
            if (count >= mCount) return;
            
            // Clear the items.
            for (int i = count; i < mCount; i++)
            {
                mItems[i] = T();
            }
            
            // TODO(bob): This never actually reallocates a smaller array.
            // Should it?
            
            // Truncate.
            mCount = count;
        }
        
        // Assigns the contents of the given array to this one. Clears this
        // array and refills it with the contents of the other.
        Array & operator=(const Array & other)
        {
            // Early out of self-assignment.
            if (&other == this) return *this;
            
            Clear();
            AddAll(other);
            
            return *this;
        }
        
        // Gets the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        T & operator[] (int index)
        {
            if (index < 0) index = mCount + index;
            ASSERT_RANGE(index, mCount);
            
            return mItems[index];
        }
        
        // Gets the item at the given index. Indexes are zero-based from the
        // beginning of the array. Negative indexes are from the end of the
        // array and go forward, so that -1 is the last item in the array.
        const T & operator[] (int index) const
        {
            if (index < 0) index = mCount + index;
            ASSERT_RANGE(index, mCount);
            
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

