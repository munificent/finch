#pragma once

#include "Macros.h"

namespace Finch
{
    class Repl
    {
    public:
        Repl()
        {}
        
        void Run();
        
    private:
        NO_COPY(Repl)
    };
}