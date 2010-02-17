#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    enum OpCode
    {
        OP_NOTHING,
        OP_NUMBER_LITERAL,
        OP_POP
    };
    
    struct Instruction
    {
        Instruction()
        :   op(OP_NOTHING)
        {}
        
        OpCode op;
        union
        {
            bool   boolean;
            double number;
        } arg;
    };
    
    // A chunk of compiled bytecode.
    class CodeBlock
    {
    public:
        CodeBlock(int capacity);
        
        ~CodeBlock();
        
        int Size() const { return mSize; }
        
        const Instruction & operator[](int i) const { return mInstructions[i]; }

        void Write(OpCode op);
        void Write(OpCode op, bool boolean);
        void Write(OpCode op, double number);
        
    private:
        Instruction * mInstructions;
        
        int mCapacity;
        int mSize;
        
        NO_COPY(CodeBlock);
    };
}