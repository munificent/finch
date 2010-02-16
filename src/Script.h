#pragma once

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class Environment;
    class Object;
    
    // Static class for loading and executing Finch code stored in script files.
    class Script
    {
    public:
        // Loads and runs the script at the given path in its own environment.
        static Ref<Object> Run(String fileName);
        
        // Loads and runs the script at the given path in the given existing
        // environment.
        static Ref<Object> Run(String fileName, Environment & env);
        
    private:
        // static class
        Script() {}
    };
}