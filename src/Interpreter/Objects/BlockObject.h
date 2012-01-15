#pragma once

#include <iostream>

#include "Block.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"
#include "Upvalue.h"

namespace Finch
{
    using std::ostream;
    
    class Block;
    
    // Object class for a block: an invokable expression and the scope that
    // encloses it.
    class BlockObject : public Object
    {
    public:
        BlockObject(const Value & parent, Ref<Block> block,
                    Ref<Object> self)
        :   Object(parent),
            mBlock(block),
            mSelf(self),
            mUpvalues(block->NumUpvalues())
        {}
        
        bool IsMethod() const { return !mSelf.IsNull(); }
        
        // Gets the object owning the method enclosing the definition of this
        // block.
        Ref<Object> Self() const { return mSelf; }

        int NumRegisters() const { return mBlock->NumRegisters(); }
        int NumParams() const { return mBlock->Params().Count(); }
        int MethodId() const { return mBlock->MethodId(); }
        
        const Ref<Object> GetConstant(int index) const;
        const Ref<Block> GetBlock(int index) const;
        
        // Gets the compiled bytecode for the block.
        const Array<Instruction> & Code() const;
        
        void AddUpvalue(Ref<Upvalue> upvalue);
        Ref<Upvalue> GetUpvalue(int index) const;
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "block";
        }
        
    private:
        Ref<Block>              mBlock;
        Ref<Object>             mSelf;
        Array<Ref<Upvalue> >    mUpvalues;
    };
}

