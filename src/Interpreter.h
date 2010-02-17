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
    
    //### bob: wip
    // Finch bytecode interpreter.
    class Interpreter
    {
    public:
        void Execute(Environment & environment, const CodeBlock & code);
        
    private:
        static const int STACK_SIZE = 1024;
        
        Stack<Ref<Object>, STACK_SIZE > mStack; 
    };
}