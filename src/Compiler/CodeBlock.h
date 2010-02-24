#pragma once

#include <iostream>
#include <vector>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    using std::vector;
    
    enum OpCode
    {
        OP_NOTHING,
        OP_NUMBER_LITERAL,
        OP_STRING_LITERAL,
        OP_BLOCK_LITERAL,
        OP_POP,
        
        OP_DEF_GLOBAL,
        OP_DEF_OBJECT,
        OP_DEF_LOCAL,
        
        OP_SET_LOCAL,
        
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
        OP_MESSAGE_10,
        
        OP_LOOP_1,
        OP_LOOP_2,
        OP_LOOP_3,
        OP_LOOP_4,
        
        OP_END_BLOCK
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
        CodeBlock(const vector<String> & params, int capacity);
        
        ~CodeBlock();
        
        const vector<String> &  Params() const { return mParams; }
        
        int Size() const { return mSize; }
        
        const Instruction & operator[](int i) const { return mInstructions[i]; }

        void Write(OpCode op);
        void Write(OpCode op, double number);
        void Write(OpCode op, int id);
        
    private:
        vector<String>  mParams;
        Instruction *   mInstructions;
        
        int mCapacity;
        int mSize;
        
        NO_COPY(CodeBlock);
    };
}