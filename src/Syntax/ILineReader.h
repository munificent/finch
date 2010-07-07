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
        
        // Gets whether or not this source can provide an infinite number of
        // lines. This is true for lines coming from an interactive session
        // (we can keep asking the user for more input), but false for files.
        // The parser uses this to decide whether to parse as much as it can or
        // to stop when it's reached a complete expression.
        virtual bool IsInfinite() const = 0;
        
        virtual bool EndOfLines() const = 0;
        virtual String NextLine() = 0;
    };
}
