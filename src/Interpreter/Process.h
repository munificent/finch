#pragma once

#include <iostream>

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "Stack.h"

namespace Finch
{
    class CodeBlock;
    class Environment;
    class Expr;
    class Interpreter;
    
    // A single bytecode execution thread in the interpreter. A Process has a
    // virtual callstack and is responsible for executing bytecode. In other
    // words, it's where stuff actually happens.
    class Process
    {
    public:
        Process(Interpreter & interpreter, Environment & environment);
        
        bool IsRunning() const { return mIsRunning; }
        
        Ref<Object> Execute(Ref<Object> block);
        
        Interpreter & GetInterpreter() { return mInterpreter; }
        Environment & GetEnvironment() { return mEnvironment; }
        
        Ref<Object> Self();

        void StopRunning() { mIsRunning = false; }
        
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
        void DiscardReturn();
        
        // Displays a runtime error to the user.
        void Error(const String & message);
        
    private:
        struct CallFrame
        {
            int               address;
            Ref<Scope>        scope;
            Ref<Object>       block;
            
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
            
            const BlockObject & Block() const { return *(block->AsBlock()); }
        };
        
        Ref<Scope>  CurrentScope() { return mCallStack.Peek().scope; }
        
        void PushOperand(Ref<Object> object);
        Ref<Object> PopOperand();
        
        bool mIsRunning;
        
        Interpreter & mInterpreter;
        Environment & mEnvironment;
        Stack<Ref<Object> > mOperands; 
        Stack<CallFrame>    mCallStack;
        
        CodeBlock mLoopCode;
        CodeBlock mDiscardCode;
        
        NO_COPY(Process);
    };
}

