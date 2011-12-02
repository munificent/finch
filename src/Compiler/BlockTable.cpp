#include "BlockTable.h"

namespace Finch
{
    int BlockTable::Add(Ref<CodeBlock> code)
    {
        //### bob: doesn't actually check for duplicates yet
        mBlocks.Add(code);
        
        return mBlocks.Count() - 1;
    }
    
    CodeBlock & BlockTable::Find(int id)
    {
        return *mBlocks[id];
    }
}

