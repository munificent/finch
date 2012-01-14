#pragma once

#include <iostream>

#include "CodeBlock.h"
#include "Expr.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"
#include "Upvalue.h"

namespace Finch
{
    using std::ostream;
    
    class BlockExemplar;
    
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
            mUpvalues(exemplar->NumUpvalues())
        {}
        
        bool IsMethod() const { return !mSelf.IsNull(); }
        
        // Gets the object owning the method enclosing the definition of this
        // block.
        Ref<Object> Self() const { return mSelf; }

        int NumRegisters() const { return mExemplar->NumRegisters(); }
        int NumParams() const { return mExemplar->Params().Count(); }
        int MethodId() const { return mExemplar->MethodId(); }
        
        const Ref<Object> GetConstant(int index) const;
        const Ref<BlockExemplar> GetExemplar(int index) const;
        
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
        Ref<BlockExemplar>      mExemplar;
        Ref<Object>             mSelf;
        Array<Ref<Upvalue> >    mUpvalues;
    };
}

