#include "Upvalue.h"

namespace Finch
{
    Value Upvalue::Get(Array<Value> & stack) const
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
    
    void Upvalue::Set(Array<Value> & stack, const Value & value)
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
    
    void Upvalue::Close(Array<Value> & stack)
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

