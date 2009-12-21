#pragma once

#include "Macros.h"
#include "Ref.h"
#include "Token.h"

namespace Finch
{
    // Interface for a class that provides a stream of Tokens.
    class ITokenSource
    {
    public:
        // Reads the next Token from the source.
        virtual Ref<Token> ReadToken() = 0;

        virtual ~ITokenSource() {}
    };
}
