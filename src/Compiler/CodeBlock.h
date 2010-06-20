#pragma once

#include <iostream>

#include "Array.h"
#include "Macros.h"
#include "String.h"

namespace Finch
{
    enum OpCode
    {                       // arg
        OP_NOTHING,         // (not used)
        OP_NUMBER_LITERAL,  // number = value
        OP_STRING_LITERAL,  // id = string table id of literal
        OP_BLOCK_LITERAL,   // id = block table id of code
        
        OP_CREATE_ARRAY,    // id = number of elements to pop
        
        OP_POP,             // (not used)
        
        OP_DEF_GLOBAL,      // id = string table id of identifier
        OP_DEF_OBJECT,      // id = string table id of identifier
        OP_DEF_LOCAL,       // id = string table id of identifier
        
        OP_SET_LOCAL,       // id = string table id of identifier
        
        OP_UNDEF_LOCAL,     // id = string table id of identifier
        OP_UNDEF_OBJECT,    // id = string table id of identifier
        OP_UNDEF_GLOBAL,    // id = string table id of identifier

        OP_LOAD_GLOBAL,     // id = string table id of identifier
        OP_LOAD_OBJECT,     // id = string table id of identifier
        OP_LOAD_LOCAL,      // id = string table id of identifier
        OP_LOAD_SELF,       // (not used)
        
        OP_MESSAGE_0,       // id = string table id of message name
        OP_MESSAGE_1,       // id = string table id of message name
        OP_MESSAGE_2,       // id = string table id of message name
        OP_MESSAGE_3,       // id = string table id of message name
        OP_MESSAGE_4,       // id = string table id of message name
        OP_MESSAGE_5,       // id = string table id of message name
        OP_MESSAGE_6,       // id = string table id of message name
        OP_MESSAGE_7,       // id = string table id of message name
        OP_MESSAGE_8,       // id = string table id of message name
        OP_MESSAGE_9,       // id = string table id of message name
        OP_MESSAGE_10,      // id = string table id of message name
        
        OP_LOOP_1,          // (not used)
        OP_LOOP_2,          // (not used)
        OP_LOOP_3,          // (not used)
        OP_LOOP_4,          // (not used)
        
        OP_END_BLOCK        // (not used)
    };
    
    // A single bytecode instruction. Each instruction is an OpCode and an
    // argument (which is unused for some ops).
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
        CodeBlock(const Array<String> & params);
        
        const Array<String> & Params() const { return mParams; }
        
        const Instruction & operator[](int i) const { return mInstructions[i]; }

        void Write(OpCode op);
        void Write(OpCode op, double number);
        void Write(OpCode op, int id);
        
    private:
        Array<String>      mParams;
        Array<Instruction> mInstructions;
        
        NO_COPY(CodeBlock);
    };
}