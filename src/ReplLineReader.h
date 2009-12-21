#pragma once

#include <iostream>

#include "Macros.h"
#include "String.h"
#include "ILineReader.h"

namespace Finch
{
    // A line reader that reads them by prompting the user and reading from
    // standard in.
    class ReplLineReader : public ILineReader
    {
    public:
        ReplLineReader()
        :   mFirstLine(true)
        {}
        
        void Reset() { mFirstLine = true; }
        
        virtual bool EndOfLines() const;
        virtual String NextLine();
        
    private:
        bool mFirstLine;
    };
}