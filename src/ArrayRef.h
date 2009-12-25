#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // Referenced-linked smart pointer.
    template <class T>
    class ArrayRef
    {
    public:
        ArrayRef()
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {}
        
        explicit ArrayRef(T obj[])
        :   mObj(obj),
            mPrev(this),
            mNext(this)
        {}
        
        ArrayRef(const ArrayRef & other)
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {
            if (&other != this)
            {
                Link(other);
            }
        }
        
        ~ArrayRef() { Clear(); }
        
        const T & operator[] (int index) const
        {
            ASSERT_NOT_NULL(mObj);
            return mObj[index];
        }
        
        T & operator[] (int index)
        {
            ASSERT_NOT_NULL(mObj);
            return mObj[index];
        }
        
        bool operator ==(const ArrayRef<T> & other) const
        {
            return mObj == other.mObj;
        }
        
        bool operator !=(const ArrayRef<T> & other) const
        {
            return mObj != other.mObj;
        }
        
        ArrayRef<T>& operator =(const ArrayRef<T> & other)
        {
            if (&other != this)
            {
                Clear();
                Link(other);
            }
            
            return *this;
        }
        
        T* Elements() const { return mObj; }
        
        bool IsNull() const { return mObj == NULL; }
        
        void Clear()
        {
            if (mNext != this)
            {
                // unlink it
                mPrev->mNext = mNext;
                mNext->mPrev = mPrev;
                
                mPrev = this;
                mNext = this;
            }
            else if (mObj != NULL)
            {
                // linked to itself, so it's the last reference
                delete [] mObj;
            }
            
            mObj = NULL;
        }
        
    private:
        
        void Link(const ArrayRef<T> & other)
        {
            // don't bother to share null
            if (other.mObj != NULL)
            {
                mObj = other.mObj;
                
                // link it in
                mPrev = other.mPrev;
                mNext = &other;
                
                other.mPrev = this;
                mPrev->mNext = this;
            }
        }
        
        T * mObj;
        
        mutable const ArrayRef<T> * mPrev;
        mutable const ArrayRef<T> * mNext;
    };
}