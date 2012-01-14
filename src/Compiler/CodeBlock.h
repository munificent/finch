#pragma once

#include <iostream>

#include "Array.h"
#include "FinchString.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"

// TODO(bob): Rename file.

#define DECODE_OP(inst) (static_cast<OpCode>((inst & 0xff000000) >> 24))
#define DECODE_A(inst)  ((inst & 0x00ff0000) >> 16)
#define DECODE_B(inst)  ((inst & 0x0000ff00) >> 8)
#define DECODE_C(inst)  (inst & 0x000000ff)

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
        OP_TAIL_MESSAGE_0,
        OP_TAIL_MESSAGE_1,
        OP_TAIL_MESSAGE_2,
        OP_TAIL_MESSAGE_3,
        OP_TAIL_MESSAGE_4,
        OP_TAIL_MESSAGE_5,
        OP_TAIL_MESSAGE_6,
        OP_TAIL_MESSAGE_7,
        OP_TAIL_MESSAGE_8,
        OP_TAIL_MESSAGE_9,
        OP_TAIL_MESSAGE_10,
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
        OP_END,           // A = register with result to return
        OP_RETURN,        // A = method id to return from,
                          // B = register with value to return
        
        // TODO(bob): These are pseudo-ops that only appear following an
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
        // Method ID for blocks that are not methods.
        static const int BLOCK_METHOD_ID = -1;
        
        // Creates a new BlockExemplar with the given parameters.
        BlockExemplar(int methodId, const Array<String> & params);
        
        int MethodId() const { return mMethodId; }
        
        // Gets the names of the parameters that this block expects.
        const Array<String> & Params() const { return mParams; }
        
        int  NumRegisters() const { return mNumRegisters; }
        void SetNumRegisters(int numRegisters) { mNumRegisters = numRegisters; }
        
        int  NumUpvalues() const { return mNumUpvalues; }
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
        const Array<Instruction> & Code() const { return mCode; }
        
        // Writes an instruction.
        void Write(OpCode op, int a = 0xff, int b = 0xff, int c = 0xff);
        
        // If the last instruction is a MESSAGE, translates it to a tail call.
        void MarkTailCall();
        
#ifdef DEBUG
        void DumpInstruction(Environment & environment, const String & prefix, Instruction instruction);
        void DebugDump(Environment & environment, const String & prefix);
#endif
        
    private:
        int                 mMethodId;
        Array<String>       mParams;
        Array<Instruction>  mCode;
        Array<Ref<Object> > mConstants;
        // Exemplars for blocks defined within this one.
        Array<Ref<BlockExemplar> > mExemplars;
        int                 mNumRegisters;
        int                 mNumUpvalues;
    };
}

