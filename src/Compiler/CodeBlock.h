#pragma once

#include <iostream>

#include "Array.h"
#include "FinchString.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

// TODO(bob): Rename file.

namespace Finch
{
    // TODO(bob): We expect this to be 32 bits. Is there a better way to specify
    // this?
    typedef unsigned int Instruction;
    
    enum OpCode
    {
        OP_CONSTANT,        // A = index of constant, B = dest register
        OP_BLOCK,           // A = index of example, B = dest register
        OP_OBJECT,          // A = parent register and dest register
        OP_ARRAY,           // A = initial capacity, B = dest register
        OP_ARRAY_ELEMENT,   // A = element register, B = dest array register
        OP_MOVE,            // A = source register, B = dest register
        OP_SELF,            // A = dest register
        OP_MESSAGE_0,       // A = index of message name in string table, 
        OP_MESSAGE_1,       // B = register of receiver (args follow),
        OP_MESSAGE_2,       // C = dest register
        OP_MESSAGE_3,
        OP_MESSAGE_4,
        OP_MESSAGE_5,
        OP_MESSAGE_6,
        OP_MESSAGE_7,
        OP_MESSAGE_8,
        OP_MESSAGE_9,
        OP_MESSAGE_10,
        OP_GET_UPVALUE,   // A = index of upvalue, B = dest reg
        OP_SET_UPVALUE,   // A = index of upvalue, B = value reg
        OP_GET_FIELD,     // A = index of field in string table, B = dest reg
        OP_SET_FIELD,     // A = index of field in string table, B = value reg
        OP_GET_GLOBAL,    // A = index of global, B = dest reg
        OP_SET_GLOBAL,    // A = index of global, B = value reg
        OP_DEF_METHOD,    // A = index of method name in string table,
                          // B = index of method body exemplar,
                          // C = object method is being defined on
        OP_DEF_FIELD,     // A = index of field name in string table,
                          // B = register with field value,
                          // C = object field is being defined on
        OP_RETURN,        // A = register with result to return
        
        // TODO(bob): There are pseudo-ops that only appear following an
        // OP_BLOCK instruction. If we want to minimize the number of ops, we
        // could reuse existing opcodes for these.
        OP_CAPTURE_LOCAL,   // A = register of local
        OP_CAPTURE_UPVALUE  // A = index of upvalue
    };
        
    // A compiled block. This contains the state that all blocks created from
    // evaluating the same chunk of code share: the compiled bytecode, constant
    // table etc. It does not contain the closure: that's owned by BlockObject.
    class BlockExemplar
    {
    public:
        // Creates a new BlockExemplar with the given parameters.
        BlockExemplar(const Array<String> & params);
        
        // Gets the names of the parameters that this block expects.
        const Array<String> & GetParams() const { return mParams; }
        
        int  GetNumRegisters() const { return mNumRegisters; }
        void SetNumRegisters(int numRegisters) { mNumRegisters = numRegisters; }
        
        int  GetNumUpvalues() const { return mNumUpvalues; }
        void SetNumUpvalues(int numUpvalues) { mNumUpvalues = numUpvalues; }
        
        // Adds the given object to the constant pool and returns its index.
        int AddConstant(Ref<Object> object);
        
        // Gets the constant at the given index in the constant pool.
        const Ref<Object> GetConstant(int index) const { return mConstants[index]; }
        
        // Adds the given exemplar to the pool and returns its index.
        int AddExemplar(Ref<BlockExemplar> exemplar);
        
        // Gets the exemplar at the given index in the pool.
        const Ref<BlockExemplar> GetExemplar(int index) const { return mExemplars[index]; }
        
        // Gets the bytecode for this block.
        const Array<Instruction> & GetCode() const { return mCode; }
        
        // Writes an instruction.
        void Write(OpCode op, int a = 0xff, int b = 0xff, int c = 0xff);
        
#ifdef DEBUG
        void DumpInstruction(Environment & environment, const String & prefix, Instruction instruction);
        void DebugDump(Environment & environment, const String & prefix);
#endif
        
    private:
        Array<String>       mParams;
        Array<Instruction>  mCode;
        Array<Ref<Object> > mConstants;
        // Exemplars for blocks defined within this one.
        Array<Ref<BlockExemplar> > mExemplars;
        int                 mNumRegisters;
        int                 mNumUpvalues;
    };

    /*
    enum OpCode
    {                       // arg
        OP_NOTHING,         // (not used)
        OP_BLOCK,           // id = id of code block
        OP_CONSTANT,        // id = index of constant
        
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
        OP_START_OBJECT,    // (not used)
        OP_END_OBJECT,      // (not used)

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

        Ref<Object> GetConstant(int index) const { return mConstants[index]; }
        Ref<CodeBlock> GetCodeBlock(int index) const { return mCodeBlocks[index]; }
        
        void Write(OpCode op);
        void Write(OpCode op, double number);
        void Write(OpCode op, int id);

        int AddConstant(Ref<Object> constant);
        int AddCodeBlock(Ref<CodeBlock> codeBlock);
        
        // Finds any message sends that immediately precede OP_END_BLOCKs and
        // changes them to tail call instructions.
        void MarkTailCalls();
        
    private:
        Array<String>           mParams;
        int                     mMethodId;
        Array<Instruction>      mInstructions;
        Array<Ref<Object> >     mConstants;
        Array<Ref<CodeBlock> >  mCodeBlocks;

        NO_COPY(CodeBlock);
    };
     */
}

