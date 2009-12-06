#pragma once

#include "Macros.h"
#include "Parser.h"

namespace Finch
{
    class Repl
    {
    public:
        Repl() {}
        
        void Run();
        
    private:
        Parser mParser;
        
        NO_COPY(Repl)
    };
}