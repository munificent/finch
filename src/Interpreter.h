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
        
        Ref<Object> Execute(const CodeBlock & code);
        
        Environment & GetEnvironment() { return mEnvironment; }
        
        Ref<Object> Self()         { return mCallStack.Peek().self; }

        void StopRunning() { mIsRunning = false; }
        
        // Pushes the given value onto the operand stack.
        void Push(Ref<Object> object);
        void PushNil();
        void PushBool(bool value);
        void PushNumber(double value);
        
        // Pushes the given block onto the call stack.
        void CallBlock(const BlockObject & block, const vector<Ref<Object> > & args);
        
        // Pushes the given method onto the call stack.
        void CallMethod(Ref<Object> self,
                        const BlockObject & block,
                        const vector<Ref<Object> > & args);
        
        void WhileLoop(Ref<Object> condition, Ref<Object> body);
        
        // Displays a runtime error to the user.
        void RuntimeError(const String & message);
        
    private:
        static const int MAX_OPERANDS = 1024;
        static const int STACK_SIZE   = 256;
        
        struct CallFrame
        {
            int               address;
            const CodeBlock * code;
            Ref<Scope>        scope;
            Ref<Object>       self;

            CallFrame()
            :   address(0),
                code(NULL),
                scope(),
                self()
            {}
            
            CallFrame(const CodeBlock * code, Ref<Scope> scope, Ref<Object> self)
            :   address(0),
                code(code),
                scope(scope),
                self(self)
            {}
        };
        
        Ref<Scope>  CurrentScope() { return mCallStack.Peek().scope; }
        
        void PushOperand(Ref<Object> object);
        Ref<Object> PopOperand();
        
        bool mIsRunning;
        
        Environment & mEnvironment;
        Stack<Ref<Object>, MAX_OPERANDS> mOperands; 
        Stack<CallFrame, STACK_SIZE>     mCallStack;

        CodeBlock mLoopCode;
        
        NO_COPY(Interpreter);
    };
}