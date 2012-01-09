#include "BlockObject.h"

namespace Finch
{
    
    const Ref<Object> BlockObject::GetConstant(int index) const
    {
        return mExemplar->GetConstant(index);
    }
    
    const Ref<BlockExemplar> BlockObject::GetExemplar(int index) const
    {
        return mExemplar->GetExemplar(index);
    }
    
    // Gets the compiled bytecode for the block.
    const Array<Instruction> & BlockObject::GetCode() const
    {
        return mExemplar->GetCode();
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
