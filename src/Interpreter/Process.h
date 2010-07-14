#pragma once

#include <iostream>

#include "CodeBlock.h"
#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Scope.h"
#include "Stack.h"

namespace Finch
{
    class Environment;
    class Expr;
    class Interpreter;
    
    //### bob: rename to Fiber
    // A single bytecode execution thread in the interpreter. A Process has a
    // virtual callstack and is responsible for executing bytecode. In other
    // words, it's where stuff actually happens.
    class Process
    {
    public:
        Process(Interpreter & interpreter, Ref<Object> block);
        
        bool IsRunning() const { return mIsRunning && !IsDone(); }
        
        bool IsDone() const;

        Ref<Object> Execute();
        
        Interpreter & GetInterpreter() { return mInterpreter; }
        Environment & GetEnvironment() { return mEnvironment; }

        void Pause() { mIsRunning = false; }
        
        // Pushes the given value onto the operand stack.
        void Push(Ref<Object> object);
        void PushNil();
        void PushBool(bool value);
        void PushNumber(double value);
        void PushString(const String & value);
        
        void CallMethod(Ref<Object> self,
                        Ref<Object> blockObj,
                        const Array<Ref<Object> > & args);
        
        // Pushes the given block onto the call stack.
        void CallBlock(Ref<Object> blockObj,
                       const Array<Ref<Object> > & args);
        
        void WhileLoop(Ref<Object> condition, Ref<Object> body);
        
        // Displays a runtime error to the user.
        void Error(const String & message);
        
    private:
        // A single stack frame on the virtual callstack.
        struct CallFrame
        {
            // The instruction pointer. Stores the index of the current bytecode instruction in the
            // block of code for this frame.
            int         address;
            
            // The local variable scope.
            Ref<Scope>  scope;
            
            // The block of code being executed by this frame.
            Ref<Object> block;
            
            CallFrame()
            :   address(0),
                scope(),
                block()
            {}
            
            CallFrame(Ref<Scope> scope, Ref<Object> block)
            :   address(0),
                scope(scope),
                block(block)
            {}
            
            // Gets the code object for this frame.
            const BlockObject & Block() const { return *(block->AsBlock()); }
        };
        
        Ref<Object> Self();
        Ref<Scope>  CurrentScope() { return mCallStack.Peek().scope; }
        
        void PushOperand(Ref<Object> object);
        Ref<Object> PopOperand();
        
        bool mIsRunning;
        
        Interpreter & mInterpreter;
        Environment & mEnvironment;
        Stack<Ref<Object> > mOperands; 
        Stack<CallFrame>    mCallStack;
        
        CodeBlock mLoopCode;
        
        NO_COPY(Process);
    };
}

