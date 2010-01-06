#pragma once

#include "Macros.h"
#include "Ref.h"
#include "Token.h"

namespace Finch
{
    class InternStringPool;
    
    // Interface for a class that provides a stream of Tokens.
    class ITokenSource
    {
    public:
        // Reads the next Token from the source.
        virtual Ref<Token> ReadToken() = 0;

        // Gets the string pool used to house strings used in the tokens.
        virtual InternStringPool & StringPool() const = 0;
        
        virtual ~ITokenSource() {}
    };
}
