#pragma once

#include "Macros.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // A reference to an identifier in an expression.
    class Identifier
    {
    public:
        Identifier(String name);
        
        String Name() const { return mName; }
        
    private:
        String mName;
    };
}

