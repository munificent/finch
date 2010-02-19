#pragma once

#include <iostream>

#include "Macros.h"

namespace Finch
{
    enum OpCode
    {
        OP_NOTHING,
        OP_NUMBER_LITERAL,
        OP_STRING_LITERAL,
        OP_BLOCK_LITERAL,
        OP_POP,
        
        OP_SET_GLOBAL,
        OP_SET_OBJECT,
        
        OP_LOAD_GLOBAL,
        OP_LOAD_OBJECT,
        OP_LOAD_LOCAL,
        
        OP_MESSAGE_0,
        OP_MESSAGE_1,
        OP_MESSAGE_2,
        OP_MESSAGE_3,
        OP_MESSAGE_4,
        OP_MESSAGE_5,
        OP_MESSAGE_6,
        OP_MESSAGE_7,
        OP_MESSAGE_8,
        OP_MESSAGE_9,
        OP_MESSAGE_10
    };
    
    struct Instruction
    {
        Instruction()
        :   op(OP_NOTHING)
        {}
        
        OpCode op;
        union
        {
            double number;
            int    id;
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
        void Write(OpCode op, double number);
        void Write(OpCode op, int id);
        
    private:
        Instruction * mInstructions;
        
        int mCapacity;
        int mSize;
        
        NO_COPY(CodeBlock);
    };
}