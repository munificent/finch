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
    
    // Finch bytecode interpreter.
    class Interpreter
    {
    public:
        Interpreter() {}
        
        Ref<Object> Execute(Environment & environment, const CodeBlock & code);
        
    private:
        static const int STACK_SIZE = 1024;
        
        Stack<Ref<Object>, STACK_SIZE> mStack; 
        
        NO_COPY(Interpreter);
    };
}