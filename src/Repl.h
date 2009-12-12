#pragma once

#include "Macros.h"
#include "FinchParser.h"

namespace Finch
{
    class Repl
    {
    public:
        Repl() {}
        
        void Run();
        
    private:
        FinchParser mParser;

        NO_COPY(Repl)
    };
}