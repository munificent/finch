#pragma once

#include <iostream>
#include <map>

#include "Macros.h"
#include "Ref.h"

namespace Finch
{
    using std::map;

    // A dictionary mapping keys to references to values.
    template <class TKey, class TValue>
    class Dictionary
    {
    public:
        // Looks up the value associated with the given key. Returns a null
        // reference if the key was not found.
        Ref<TValue> Find(TKey key)
        {
            typename map<TKey, Ref<TValue> >::iterator found = mMap.find(key);
            
            if (found != mMap.end()) return found->second;
            
            return Ref<TValue>();
        }
        
        // Inserts the given value at the given key. Returns the previous value
        // with that key, or an empty reference if the key was not found.
        Ref<TValue> Insert(TKey key, Ref<TValue> value)
        {
            typename map<TKey, Ref<TValue> >::iterator found = mMap.lower_bound(key);
            
            Ref<TValue> oldValue;
            
            if ((found != mMap.end()) && !(mMap.key_comp()(key, found->first)))
            {
                // key already present, so get the old value then replace it
                oldValue = found->second;
                found->second = value;
            }
            else
            {
                // key not present, so add it now
                mMap.insert(found, typename map<TKey, Ref<TValue> >::value_type(key, value));
            }
            
            return oldValue;
        }
        
        // Replaces the value at the given key. If the key is not already
        // present, does nothing and returns an empty reference. Otherwise, it
        // replaces the value at that key and returns the old value.
        Ref<TValue> Replace(TKey key, Ref<TValue> value)
        {
            typename map<TKey, Ref<TValue> >::iterator found = mMap.lower_bound(key);
            
            Ref<TValue> oldValue;
            
            if ((found != mMap.end()) && !(mMap.key_comp()(key, found->first)))
            {
                // key present, so get the old value then replace it
                oldValue = found->second;
                found->second = value;
            }
            
            return oldValue;
        }
        
        // Removes the value with the given key. Returns true if the key was
        // found and removed.
        bool Remove(TKey key)
        {
            typename map<TKey, Ref<TValue> >::iterator found = mMap.find(key);

            if (found != mMap.end())
            {
                mMap.erase(found);
                return true;
            }
            
            return false;
        }
        
        // Removes all items from the Dictionary.
        void Clear()
        {
            mMap.clear();
        }
        
    private:
        map<TKey, Ref<TValue> > mMap;
    };
}