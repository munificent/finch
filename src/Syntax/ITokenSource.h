#pragma once

#include "Macros.h"
#include "Ref.h"
#include "Token.h"

namespace Finch
{
    // Abstract interface for a class that provides a stream of tokens.
    class ITokenSource
    {
    public:
        virtual Ref<Token> ReadToken() = 0;

        virtual ~ITokenSource() {}
    };
}
