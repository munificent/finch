#include "Macros.h"

namespace Finch
{
    class RefPool
    {
    public:
        static void Initialize();
    
    protected:
        struct CountedObject
        {
            int    refCount;
            
            // if refCount > 0 then this will point to the ref counted object
            // otherwise, it will point to the next available CountedObject
            void * data;
        };
        
        static CountedObject* Add(void * object);
        
    private:
        static const int MAX_COUNTED_OBJECTS = 1024;
        
        static CountedObject   sCounted[MAX_COUNTED_OBJECTS];
        static CountedObject * sNextFree;
    };

    // Referenced-counter smart pointer.
    template <class T>
    class Ref : private RefPool
    {
    public:
        Ref(T * obj)
        :   mCounted(RefPool::Add(obj))
        {}
        
        Ref(const Ref & other)
        :   mCounted(other.mCounted)
        {
            mCounted->refCount++;
        }
        
        ~Ref()
        {
            mCounted->refCount--;
            
            if (mCounted->refCount == 0)
            {
                //### bob: should allow custom allocator at some point
                delete reinterpret_cast<T*>(mCounted->data);
            }
        }

        T & operator *() const
        {
            return *reinterpret_cast<T*>(mCounted->data);
        }
        
        //### bob: not implemented yet
        Ref& operator =(const Ref & other);
        
        int Count() const { return mCounted->refCount; }
        
    private:        
        CountedObject * mCounted;
    };
}