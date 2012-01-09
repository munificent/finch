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
        
        const Ref<Object> GetConstant(int index) const { return mExemplar->GetConstant(index); }
        const Ref<BlockExemplar> GetExemplar(int index) const { return mExemplar->GetExemplar(index); }
        
        // Gets the compiled bytecode for the block.
        const Array<Instruction> & GetCode() const { return mExemplar->GetCode(); } 
        
        void AddUpvalue(Ref<Object> upvalue) { mUpvalues.Add(upvalue); }
        Ref<Object> GetUpvalue(int index) const { return mUpvalues[index]; }
        
        virtual BlockObject * AsBlock() { return this; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "block";
        }
        
    private:
        Ref<BlockExemplar>   mExemplar;
        Ref<Object>          mSelf;
        Array<Ref<Object> >  mUpvalues;
    };
}

