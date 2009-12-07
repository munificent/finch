#pragma once

#include <iostream>

#include "Macros.h"
#include "String.h"

namespace Finch
{
    class Expr
    {
    public:
        virtual void Trace(std::ostream & stream) const = 0;
    };
    
    std::ostream& operator<<(std::ostream& cout, const Expr & expr);
}