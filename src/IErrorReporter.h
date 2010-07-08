#pragma once

#include <iostream>

#include "Macros.h"
#include "FinchString.h"

namespace Finch
{
    // Interface for a class that can report errors. Used to keep the parser
    // from being directly coupled to the interpreter or host which do the
    // actual error reporting.
    class IErrorReporter
    {
    public:
        virtual ~IErrorReporter() {}
        
        // Reports the given error message.
        virtual void Error(String message) = 0;
    };
}
