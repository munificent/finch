#include "BlockObject.h"

namespace Finch
{
    
    const Value & BlockObject::GetConstant(int index) const
    {
        return mBlock->GetConstant(index);
    }
    
    const Ref<Block> BlockObject::GetBlock(int index) const
    {
        return mBlock->GetBlock(index);
    }
    
    // Gets the compiled bytecode for the block.
    const Array<Instruction> & BlockObject::Code() const
    {
        return mBlock->Code();
    }
    
    void BlockObject::AddUpvalue(Ref<Upvalue> upvalue)
    {
        mUpvalues.Add(upvalue);
    }
    
    Ref<Upvalue> BlockObject::GetUpvalue(int index) const
    {
        return mUpvalues[index];
    }
}
