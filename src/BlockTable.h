#pragma once

#include "Object.h"
#include "Ref.h"

namespace Finch
{
    //### bob: the implementation of this just a temp prototype.
    class BlockTable
    {
    public:
        // Adds the given block to the table if not already present, and
        // returns its ID.
        int Add(Ref<Object> block);
        
        // Looks up the block with the given ID in the table.
        Ref<Object> Find(int id);
        
    private:
        static const int MAX_BLOCKS = 4096; //### bob: arbitrary
        
        Ref<Object> mBlocks[MAX_BLOCKS];
        int         mNumBlocks;
    };
}