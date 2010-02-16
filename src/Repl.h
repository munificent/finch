#pragma once

#include "Macros.h"

namespace Finch
{
    // A simple read/eval/print loop. Calling Run() will create a new Finch
    // environment, read an expression from stdin, evaluate it, and repeat
    // until the user quits by calling "Ether quit".
    class Repl
    {
    public:
        Repl()
        {}
        
        // Runs the REPL. This function will not return until the user quits.
        void Run();
        
    private:
        NO_COPY(Repl);
    };
}