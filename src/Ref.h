#include "Macros.h"

namespace Finch
{
    // Referenced-linked smart pointer.
    template <class T>
    class Ref
    {
    public:
        explicit Ref(T * obj)
        :   mObj(obj),
            mPrev(this),
            mNext(this)
        {}
        
        Ref(Ref & other)
        :   mObj(other.mObj),
            mPrev(other.mPrev),
            mNext(&other)
        {
            // link it in
            other.mPrev = this;
            mPrev->mNext = this;
        }
        
        ~Ref()
        {
            if (mNext != this)
            {
                // unlink it
                mPrev->mNext = mNext;
                mNext->mPrev = mPrev;
            }
            else
            {
                // linked to itself, so it's the last reference
                delete mObj;
            }
        }

        T & operator *() const
        {
            return *mObj;
        }
        
        //### bob: not implemented yet
        Ref& operator =(const Ref & other);
        
    private:        
        T * mObj;
        
        Ref<T> * mPrev;
        Ref<T> * mNext;
    };
}