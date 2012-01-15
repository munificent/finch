#pragma once

#include "Block.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Stack.h"
#include "Upvalue.h"

//#define TRACE_INSTRUCTIONS

namespace Finch
{
    class Environment;
    class Expr;
    class Interpreter;
    
    // A single bytecode execution thread in the interpreter. A Fiber has a
    // virtual callstack and is responsible for executing bytecode. In other
    // words, it's where stuff actually happens.
    class Fiber
    {
    public:
        Fiber(Interpreter & interpreter, const Value & block);

        bool IsRunning() const { return mIsRunning && !IsDone(); }
        
        bool IsDone() const;

        Value Execute();
        
        Interpreter & GetInterpreter() { return mInterpreter; }

        void Pause() { mIsRunning = false; }

        const Value & Nil();
        const Value & CreateBool(bool value);
        Value CreateNumber(double value);
        Value CreateString(const String & value);
        
        // Pushes the given block onto the call stack.
        void CallBlock(const Value & receiver, const Value & blockObj, ArgReader & args);

        // Displays a runtime error to the user.
        void Error(const String & message);
        
        // Gets the current number of stack frames on the callstack. Used as a
        // diagnostic to ensure that tail call optimization is working.
        int GetCallstackDepth() const;
    private:
        // A single stack frame on the virtual callstack.
        struct CallFrame
        {
            // The instruction pointer. Stores the index of the current bytecode
            // instruction in the bytecode for this frame.
            int ip;
            
            // The index on the stack of the first register for this frame.
            int stackStart;

            // The current receiver.
            Value receiver;
            
            // The block of code being executed by this frame.
            Value block;
            
            CallFrame()
            :   ip(0),
                stackStart(0),
                receiver(),
                block()
            {}
            
            CallFrame(int stackStart, const Value & receiver, const Value & block)
            :   ip(0),
                stackStart(stackStart),
                receiver(receiver),
                block(block)
            {}

            // Gets the code object for this frame.
            const BlockObject & Block() const { return *(block.AsBlock()); }
        };
        
        // Loads a register for the given callframe.
        Value Load(const CallFrame & frame, int reg);
        
        // Stores a register for the given callframe.
        void Store(const CallFrame & frame, int reg, const Value & value);

        void PopCallFrame();
        void StoreMessageResult(const Value & result);

        Value SendMessage(StringId messageId, int receiverReg, int numArgs);

        const Value & Self();
        
        Ref<Upvalue> CaptureUpvalue(int stackIndex);
        
#ifdef TRACE_INSTRUCTIONS
        void TraceInstruction(Instruction instruction);
        void TraceStack();
#endif
        
        bool mIsRunning;
        Interpreter & mInterpreter;
        Array<Value>  mStack;
        Stack<CallFrame>     mCallFrames;
        
        // Reference to first upvalue in list of open upvalues. List is ordered
        // from top of stack down.
        Ref<Upvalue> mOpenUpvalues;
        
        NO_COPY(Fiber);
    };
}

