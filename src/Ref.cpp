#include <memory>

#include "Macros.h"
#include "Ref.h"

namespace Finch
{
    RefPool::CountedObject   RefPool::sCounted[RefPool::MAX_COUNTED_OBJECTS];
    RefPool::CountedObject * RefPool::sNextFree;
    
    void RefPool::Initialize()
    {
        // set up the object pool
        for (int i = 0; i < MAX_COUNTED_OBJECTS; i++)
        {
            sCounted[i].refCount = 0;
            
            // each points to the next, except for the last which terminates
            sCounted[i].data = 
                (i == MAX_COUNTED_OBJECTS - 1) ? NULL : &sCounted[i + 1];
        }
        
        // the first free one is the beginning of the pool
        sNextFree = &sCounted[0];
    }

    RefPool::CountedObject* RefPool::Add(void * object)
    {
        ASSERT(sNextFree != NULL, "Cannot create any new ref-counted objects "
                                  "because the pool is full.");
        
        // pull the head off the free list
        CountedObject * counted = sNextFree;
        sNextFree = reinterpret_cast<CountedObject *>(sNextFree->data);
        
        // initialize the new one
        counted->refCount = 1;
        counted->data     = object;
        
        return counted;
    }
}