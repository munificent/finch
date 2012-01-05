#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // A dictionary mapping keys to values. Uses a dynamically-grown hashtable.
    // Both TKey and TValue must have default constructors as well as support
    // copying. TKey must also have a HashCode() method.
    template <class TKey, class TValue>
    class Dictionary
    {
    public:
        Dictionary()
        :   mTable(NULL),
            mCount(0),
            mTableSize(0)
        {}
        
        ~Dictionary()
        {
            if (mTable != NULL)
            {
                delete [] mTable;
            }
        }
        
        // Looks up the value associated with the given key. Returns a null
        // reference if the key was not found.
        bool Find(const TKey & key, TValue * value)
        {
            int index = FindIndex(key);            
            
            if (index == -1) return false;
            
            *value = mTable[index].value;
            return true;
        }
        
        // Inserts the given value at the given key.
        void Insert(const TKey & key, const TValue & value)
        {
            mCount++;
            EnsureCapacity();
            
            // figure out where to insert it in the table. use open addressing
            // and basic linear probing
            int index = static_cast<int>(key.HashCode() & 0x7fffffff) % mTableSize;

            // note that we shouldn't have to worry about an infinite loop here
            // the previous call will ensure there are open spaces in the table
            //### bob: hack. using .Length() here assumes TKey is string
            while ((mTable[index].key.Length() > 0) &&
                   (mTable[index].key != key))
            {
                index = (index + 1) % mTableSize;
            }
            
            // insert it into the table
            mTable[index].key   = key;
            mTable[index].value = value;
        }
        
        // Replaces the value at the given key. If the key is not already
        // present, does nothing and returns false. Otherwise, it
        // replaces the value at that key and returns true.
        bool Replace(const TKey & key, const TValue & value)
        {
            int index = FindIndex(key);
            
            // not found
            if (index == -1) return false;
            
            // replace the value
            mTable[index].value = value;
            
            return true;
        }
        
        // Removes the value with the given key. Returns true if the key was
        // found and removed.
        bool Remove(const TKey & key)
        {
            int index = FindIndex(key);
            
            // not found
            if (index == -1) return false;
            
            // remove the item
            mTable[index].key   = TKey();
            mTable[index].value = TValue();
            
            mCount--;
            return true;
        }
        
        // Removes all items from the Dictionary.
        void Clear()
        {
            mCount = 0;
            mTableSize = 0;
            delete [] mTable;
        }
        
    private:
        // Gets the index of the item with the given key in the table, or -1
        // if not found.
        int FindIndex(const TKey & key)
        {
            // can't find it in an empty table
            if (mTableSize == 0) return -1;
            
            int index = static_cast<int>(key.HashCode() & 0x7fffffff) % mTableSize;
            
            while (true)
            {
                // stop if we found it
                if (mTable[index].key == key) return index;
                
                // if we found an empty slot, the item must not be in the table
                //### bob: hack. using .Length() here assumes TKey is string
                if (mTable[index].key.Length() == 0) return -1;
                
                // try the next slot
                index = (index + 1) % mTableSize;
            }
        }
        
        void EnsureCapacity()
        {
            // only resize if we're too full
            if (mCount <= mTableSize * MAX_LOAD_PERCENT / 100) return;

            // figure out the new table size
            int oldSize = mTableSize;
            mTableSize = MIN_CAPACITY;
            if (oldSize >= MIN_CAPACITY)
            {
                mTableSize = oldSize * GROW_FACTOR;
            }
            
            // create the new hashtable
            Pair * oldTable = mTable;
            mTable = new Pair[mTableSize];
            
            // move the existing items over
            if (oldTable != NULL)
            {
                for (int i = 0; i < oldSize; i++)
                {
                    //### bob: hack. using .Length() here assumes TKey is string
                    if (oldTable[i].key.Length() > 0)
                    {
                        Insert(oldTable[i].key, oldTable[i].value);
                    }
                }
                
                delete [] oldTable;
            }
        }
        
        // What percentage of the table should be filled with values before
        // it is resized.
        static const int MAX_LOAD_PERCENT = 75;
        
        static const int MIN_CAPACITY = 16;
        static const int GROW_FACTOR  = 2;

        struct Pair
        {
            TKey   key;
            TValue value;
        };
        
        Pair * mTable;
        int    mCount;     // number of items stored in the table
        int    mTableSize; // size of the table itself
        
        NO_COPY(Dictionary);
    };

    // TODO(bob): Total hack. Copy and pasting Dictionary so that we can use
    // int keys is lame!
    // A dictionary mapping non-negative ints to values. Uses a
    // dynamically-grown hashtable. TValue must have a default constructor as
    // well as support copying.
    template <class TValue>
    class IdTable
    {
    public:
        IdTable()
        :   mTable(NULL),
            mCount(0),
            mTableSize(0)
        {}
        
        ~IdTable()
        {
            if (mTable != NULL)
            {
                delete [] mTable;
            }
        }
        
        // Looks up the value associated with the given key. Returns a null
        // reference if the key was not found.
        bool Find(int key, TValue * value)
        {
            int index = FindIndex(key);            
            
            if (index == -1) return false;
            
            *value = mTable[index].value;
            return true;
        }
        
        // Inserts the given value at the given key.
        void Insert(int key, const TValue & value)
        {
            mCount++;
            EnsureCapacity();
            
            // figure out where to insert it in the table. use open addressing
            // and basic linear probing
            int index = static_cast<int>(key & 0x7fffffff) % mTableSize;
            
            // note that we shouldn't have to worry about an infinite loop here
            // the previous call will ensure there are open spaces in the table
            while ((mTable[index].key > 0) &&
                   (mTable[index].key != key))
            {
                index = (index + 1) % mTableSize;
            }
            
            // insert it into the table
            mTable[index].key   = key;
            mTable[index].value = value;
        }
        
        // Replaces the value at the given key. If the key is not already
        // present, does nothing and returns false. Otherwise, it
        // replaces the value at that key and returns true.
        bool Replace(int key, const TValue & value)
        {
            int index = FindIndex(key);
            
            // not found
            if (index == -1) return false;
            
            // replace the value
            mTable[index].value = value;
            
            return true;
        }
        
        // Removes the value with the given key. Returns true if the key was
        // found and removed.
        bool Remove(int key)
        {
            int index = FindIndex(key);
            
            // not found
            if (index == -1) return false;
            
            // remove the item
            mTable[index].key   = EMPTY;
            mTable[index].value = TValue();
            
            mCount--;
            return true;
        }
        
        // Removes all items from the Dictionary.
        void Clear()
        {
            mCount = 0;
            mTableSize = 0;
            delete [] mTable;
        }
        
    private:
        // Key value that indicates an empty cell in the table.
        static const int EMPTY = -1;
        
        // Gets the index of the item with the given key in the table, or -1
        // if not found.
        int FindIndex(int key)
        {
            // can't find it in an empty table
            if (mTableSize == 0) return -1;
            
            int index = static_cast<int>(key & 0x7fffffff) % mTableSize;
            
            while (true)
            {
                // stop if we found it
                if (mTable[index].key == key) return index;
                
                // if we found an empty slot, the item must not be in the table
                if (mTable[index].key == EMPTY) return -1;
                
                // try the next slot
                index = (index + 1) % mTableSize;
            }
        }
        
        void EnsureCapacity()
        {
            // only resize if we're too full
            if (mCount <= mTableSize * MAX_LOAD_PERCENT / 100) return;
            
            // figure out the new table size
            int oldSize = mTableSize;
            mTableSize = MIN_CAPACITY;
            if (oldSize >= MIN_CAPACITY)
            {
                mTableSize = oldSize * GROW_FACTOR;
            }
            
            // create the new hashtable
            Pair * oldTable = mTable;
            mTable = new Pair[mTableSize];
            
            for (int i = 0; i < mTableSize; i++)
            {
                mTable[i].key = EMPTY;
            }
            
            // move the existing items over
            if (oldTable != NULL)
            {
                for (int i = 0; i < oldSize; i++)
                {
                    if (oldTable[i].key >= 0)
                    {
                        Insert(oldTable[i].key, oldTable[i].value);
                    }
                }
                
                delete [] oldTable;
            }
        }
        
        // What percentage of the table should be filled with values before
        // it is resized.
        static const int MAX_LOAD_PERCENT = 75;
        
        static const int MIN_CAPACITY = 16;
        static const int GROW_FACTOR  = 2;
        
        struct Pair
        {
            int    key;
            TValue value;
        };
        
        Pair * mTable;
        int    mCount;     // number of items stored in the table
        int    mTableSize; // size of the table itself
        
        NO_COPY(IdTable);
    };
}

