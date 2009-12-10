#pragma once

#include "Macros.h"
#include "FinchParser.h"
#include "Evaluator.h"

namespace Finch
{
    class Repl
    {
    public:
        Repl() {}
        
        void Run();
        
    private:
        FinchParser mParser;
        Evaluator   mEvaluator;
        
        NO_COPY(Repl)
    };
}