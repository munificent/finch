#include "Upvalue.h"

namespace Finch
{
    Value Upvalue::Get(Array<Ref<Object> > & stack) const
    {
        if (IsOpen())
        {
            return Value::HackWrapRef(stack[mStackIndex]);
        }
        else
        {
            return mValue;
        }
    }
    
    void Upvalue::Set(Array<Ref<Object> > & stack, const Value & value)
    {
        if (IsOpen())
        {
            stack[mStackIndex] = value.Obj();
        }
        else
        {
            mValue = value;
        }
    }
    
    void Upvalue::Close(Array<Ref<Object> > & stack)
    {
        // Capture the value.
        mValue = Value::HackWrapRef(stack[mStackIndex]);
        
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

