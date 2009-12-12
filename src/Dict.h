#pragma once

#include <iostream>
#include <map>

#include "Macros.h"
#include "Ref.h"

namespace Finch
{
    //### bob: might be nice to use this instead of map, but need to
    // investigate more to see if i can actually make a more usable interface.
    /*
    using std::map;
    
    template <class TKey, TValue>
    class Dict
    {
    public:            
        void Clear()
        {
            mMap.clear();
        }
        
    private:
        map mMap<TKey, Ref<TValue> >;
    };*/
}