#pragma once

#include "Token.h"

namespace Finch
{
    class ITokenReader
    {
    public:
        virtual void Read(Token token) = 0;
    };
}
