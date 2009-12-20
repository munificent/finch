#pragma once

#include <iostream>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    class ILineReader
    {
    public:
        virtual void Start() = 0;
        virtual bool EndOfLines() const = 0;
        virtual String NextLine() = 0;
        
        virtual ~ILineReader() {}
    };
}