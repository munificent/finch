#pragma once

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class Interpreter;
    class Object;
    
    // Static class for loading and executing Finch code stored in script files.
    class Script
    {
    public:
        // Loads and runs the script at the given path in its own interpreter.
        static void Run(String fileName);
        
        static void Execute(String fileName, Interpreter & interpreter);
        
        // Loads and runs the script at the given path in the given existing
        // interpreter.
        static void Run(String fileName, Interpreter & interpreter);
        
    private:
        // static class
        Script() {}
    };
}

