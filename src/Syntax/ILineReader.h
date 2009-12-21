#pragma once

#include <iostream>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    // Interface for a class that reads a series of lines of text from some
    // source. Used to abstract where the Lexer gets it Finch source from so
    // that we can use it both for the Repl and for parsing entire source files.
    class ILineReader
    {
    public:
        virtual bool EndOfLines() const = 0;
        virtual String NextLine() = 0;
        
        virtual ~ILineReader() {}
    };
}