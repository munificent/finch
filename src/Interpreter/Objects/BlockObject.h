#pragma once

#include <iostream>

#include "CodeBlock.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    class BlockExemplar;
    
    // TODO(bob): Move into separate file.
    // TODO(bob): If we get rid of Ref<T> and use pointers and a more direct
    // value representation, this can be much simpler and we can get rid of
    // the weird passing in the stack thing.
    class Upvalue
    {
    public:
        // Default constructor so we can use it in Array<T>.
        Upvalue()
        :   mStackIndex(-1)
        {}
        
        Upvalue(int stackIndex)
        :   mStackIndex(stackIndex)
        {}
        
        Ref<Object> Get(Array<Ref<Object> > & stack) const
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
        
        void Set(Array<Ref<Object> > & stack, Ref<Object> value)
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
        
        void Close(Array<Ref<Object> > & stack)
        {
            // Capture the value.
            mValue = stack[mStackIndex];
            
            // Detach from the stack.
            mStackIndex = -1;
        }
        
        int Index() const
        {
            return mStackIndex;
        }
        
        bool IsOpen() const
        {
            return mStackIndex != -1;
        }
        
    private:
        int mStackIndex;    // Will be -1 if Upvalue is closed.
        Ref<Object> mValue; // Only use when Upvalue is closed.
    };
    
    // Object class for a block: an invokable expression and the scope that
    // encloses it.
    class BlockObject : public Object
    {
    public:
        BlockObject(Ref<Object> parent, Ref<BlockExemplar> exemplar,
                    Ref<Object> self)
        :   Object(parent),
            mExemplar(exemplar),
            mSelf(self),
            mUpvalues(exemplar->GetNumUpvalues())
        {}
        
        bool IsMethod() const { return !mSelf.IsNull(); }
        
        // Gets the object owning the method enclosing the definition of this
        // block.
        Ref<Object> Self() const { return mSelf; }

        int GetNumRegisters() const { return mExemplar->GetNumRegisters(); }
        
        const Ref<Object> GetConstant(int index) const;
        const Ref<BlockExemplar> GetExemplar(int index) const;
        
        // Gets the compiled bytecode for the block.
        const Array<Instruction> & GetCode() const;
        
        void AddUpvalue(Ref<Upvalue> upvalue);
        Ref<Upvalue> GetUpvalue(int index) const;
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "block";
        }
        
    private:
        Ref<BlockExemplar>      mExemplar;
        Ref<Object>             mSelf;
        Array<Ref<Upvalue> >    mUpvalues;
    };
}

