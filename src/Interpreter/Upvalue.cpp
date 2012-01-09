#include "Upvalue.h"

namespace Finch
{
    Ref<Object> Upvalue::Get(Array<Ref<Object> > & stack) const
    {
        if (IsOpen())
        {
            return stack[mStackIndex];
        }
        else
        {
            return mValue;
        }
    }
    
    void Upvalue::Set(Array<Ref<Object> > & stack, Ref<Object> value)
    {
        if (IsOpen())
        {
            stack[mStackIndex] = value;
        }
        else
        {
            mValue = value;
        }
    }
    
    void Upvalue::Close(Array<Ref<Object> > & stack)
    {
        // Capture the value.
        mValue = stack[mStackIndex];
        
        // Detach from the stack.
        mStackIndex = -1;
    }
    
    int Upvalue::Index() const
    {
        return mStackIndex;
    }
    
    bool Upvalue::IsOpen() const
    {
        return mStackIndex != -1;
    }
}

