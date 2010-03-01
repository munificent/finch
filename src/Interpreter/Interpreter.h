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
    
    // Finch bytecode interpreter.
    class Interpreter
    {
    public:
        Interpreter(Environment & environment);
        
        bool IsRunning() const { return mIsRunning; }
        
        Ref<Object> Execute(Ref<Object> block);
        
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
        void RuntimeError(const String & message);
        
    private:
        static const int MAX_OPERANDS = 2048;
        static const int STACK_SIZE   = 4096;
        
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
            
            BlockObject & Block() { return *(block->AsBlock()); }
        };
        
        Ref<Scope>  CurrentScope() { return mCallStack.Peek().scope; }
        
        void PushOperand(Ref<Object> object);
        Ref<Object> PopOperand();
        
        bool mIsRunning;
        
        Environment & mEnvironment;
        Stack<Ref<Object>, MAX_OPERANDS> mOperands; 
        Stack<CallFrame, STACK_SIZE>     mCallStack;
        
        CodeBlock mLoopCode;
        CodeBlock mDiscardCode;
        
        NO_COPY(Interpreter);
    };
}