#pragma once

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    class Interpreter;
    class Object;
    class Process;
    
    // Static class for loading and executing Finch code stored in script files.
    class Script
    {
    public:
        // Loads and runs the script at the given path in its own process.
        static void Run(String fileName);
        
        static void Execute(String fileName, Interpreter & interpreter);
        
    private:
        // static class
        Script() {}
    };
}

