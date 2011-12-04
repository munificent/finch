#pragma once

#include <iostream>

#include "Array.h"
#include "Macros.h"
#include "FinchString.h"

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
        OP_DUP,             // (not used)

        OP_DEF_LOCAL,       // id = string table id of identifier
        OP_DEF_FIELD,       // id = string table id of identifier

        OP_SET_LOCAL,       // id = string table id of identifier
        OP_SET_FIELD,       // id = string table id of identifier

        OP_UNDEF_LOCAL,     // id = string table id of identifier
        OP_UNDEF_FIELD,     // id = string table id of identifier

        OP_LOAD_LOCAL,      // id = string table id of identifier
        OP_LOAD_FIELD,      // id = string table id of identifier
        OP_LOAD_SELF,       // (not used)

        OP_BIND_METHOD,     // id = method name
        OP_BIND_OBJECT,     // id = variable name
        OP_MAKE_OBJECT,     // (not used)

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
        
        OP_TAIL_MESSAGE_0,  // id = string table id of message name
        OP_TAIL_MESSAGE_1,  // id = string table id of message name
        OP_TAIL_MESSAGE_2,  // id = string table id of message name
        OP_TAIL_MESSAGE_3,  // id = string table id of message name
        OP_TAIL_MESSAGE_4,  // id = string table id of message name
        OP_TAIL_MESSAGE_5,  // id = string table id of message name
        OP_TAIL_MESSAGE_6,  // id = string table id of message name
        OP_TAIL_MESSAGE_7,  // id = string table id of message name
        OP_TAIL_MESSAGE_8,  // id = string table id of message name
        OP_TAIL_MESSAGE_9,  // id = string table id of message name
        OP_TAIL_MESSAGE_10, // id = string table id of message name
        
        OP_RETURN,          // id = method id of enclosing method

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

    // A block expression compiled to bytecode for a simple stack-based virtual
    // machine. This is what the interpreter executes.
    class CodeBlock
    {
    public:
        // Creates a new CodeBlock for a block that takes parameters with the
        // given names.
        CodeBlock(const Array<String> & params, int methodId);
        
        // Gets the names of the parameters that this block expects.
        const Array<String> & Params() const { return mParams; }

        int MethodId() const { return mMethodId; }

        // Gets the instruction at the given offset in the block.
        const Instruction & operator[](int i) const { return mInstructions[i]; }

        void Write(OpCode op);
        void Write(OpCode op, double number);
        void Write(OpCode op, int id);

        // Finds any message sends that immediately precede OP_END_BLOCKs and
        // changes them to tail call instructions.
        void MarkTailCalls();
        
    private:        
        Array<String>      mParams;
        int                mMethodId;
        Array<Instruction> mInstructions;

        NO_COPY(CodeBlock);
    };
}

