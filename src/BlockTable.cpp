#include "BlockTable.h"

namespace Finch
{
    int BlockTable::Add(Ref<Object> block)
    {
        //### bob: doesn't actually check for duplicates yet
        mBlocks[mNumBlocks] = block;
        
        return mNumBlocks++;
    }
    
    Ref<Object> BlockTable::Find(int id)
    {
        ASSERT_INDEX(id, mNumBlocks);
        
        return mBlocks[id];
    }
}
