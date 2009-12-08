#pragma once

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
        
        Ref(const Ref & other)
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {
            Link(other);
        }
        
        ~Ref() { Clear(); }

        T & operator *() const { return *mObj; }
        T * operator ->() const { return mObj; }
        
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
}