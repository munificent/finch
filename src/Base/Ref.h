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
        // Constructs a new null pointer.
        Ref()
        :   mObj(NULL),
            mPrev(this),
            mNext(this)
        {}

        // Wraps the given raw pointer in a new smart pointer. This should only
        // be used once for any given pointer (i.e. you wrap the raw pointer as
        // soon as its allocated and then only access it through Ref<T> from
        // that point on.
        explicit Ref(T * obj)
        :   mObj(obj),
            mPrev(this),
            mNext(this)
        {}
        
        // Copies a reference. Both references will refer to the same object.
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
        
        // Compares two references. References are equal if they refer to the
        // same object.
        bool operator ==(const Ref<T> & other) const
        {
            return mObj == other.mObj;
        }
        
        // Compares two references. References are not equal if they refer to
        // different objects.
        bool operator !=(const Ref<T> & other) const
        {
            return mObj != other.mObj;
        }
        
        // Discards the currently referred to object and assigns the given
        // reference to this one.
        Ref<T>& operator =(const Ref<T> & other)
        {
            if (&other != this)
            {
                Clear();
                Link(other);
            }
            
            return *this;
        }
        
        // Gets whether or not this reference is pointing to null.
        bool IsNull() const { return mObj == NULL; }
        
        // Clears the reference. If this was the last reference to the referred
        // object, it will be deallocated.
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

