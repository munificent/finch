#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    // Referenced-linked smart pointer.
    template <class T>
    class Ref
    {
    public:
        Ref()
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {}
            
        explicit Ref(T * obj)
        :   mObj(obj),
            mPrev(this),
            mNext(this)
        {}
        
        Ref(const Ref<T> & other)
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {
            if (&other != this)
            {
                Link(other);
            }
        }
        
        ~Ref() { Clear(); }

        T & operator *() const { return *mObj; }
        T * operator ->() const { return mObj; }
        
        bool operator ==(const Ref<T> & other) const
        {
            return mObj == other.mObj;
        }
        
        bool operator !=(const Ref<T> & other) const
        {
            return mObj != other.mObj;
        }
        
        Ref<T>& operator =(const Ref<T> & other)
        {
            if (&other != this)
            {
                Clear();
                Link(other);
            }
            
            return *this;
        }
        
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
                delete mObj;
            }
            
            mObj = NULL;
        }
        
    private:

        void Link(const Ref<T> & other)
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
        
        mutable const Ref<T> * mPrev;
        mutable const Ref<T> * mNext;
    };
    
    template <class T>
    std::ostream& operator<<(std::ostream& cout, const Ref<T> & ref)
    {
        if (ref.IsNull())
        {
            cout << "(null reference)";
        }
        else
        {
            cout << *ref;
        }
        
        return cout;
    }
}