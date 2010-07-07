#pragma once

#include "Macros.h"
#include "Token.h"

namespace Finch
{
    // Interface for a class that provides a stream of Tokens.
    class ITokenSource
    {
    public:
        // Gets whether or not this source can provide an infinite number of
        // tokens. This is true for tokens coming from an interactive session
        // (we can keep asking the user for more input), but false for files.
        // The parser uses this to decide whether to parse as much as it can or
        // to stop when it's reached a complete expression.
        virtual bool IsInfinite() const = 0;
        
        // Reads the next Token from the source.
        virtual Token ReadToken() = 0;

        virtual ~ITokenSource() {}
    };
}

