#pragma once

#include "Macros.h"
#include "Object.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    class Environment;
    class Object;
    
    class Script
    {
    public:
        static Ref<Object> Run(String fileName);
        static Ref<Object> Run(String fileName, Environment & env);
        
    private:
        // static class
        Script() {}
    };
}