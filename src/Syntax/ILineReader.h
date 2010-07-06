#pragma once

#include <iostream>

#include "Macros.h"
#include "FinchString.h"

namespace Finch
{
    // Interface for a class that reads a series of lines of text from some
    // source. Used to abstract where the Lexer gets its Finch source code from
    // so that we can use it both for the Repl and for parsing entire source
    // files.
    class ILineReader
    {
    public:
        virtual ~ILineReader() {}
        
        virtual bool EndOfLines() const = 0;
        virtual String NextLine() = 0;
    };
}
