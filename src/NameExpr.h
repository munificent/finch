#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "String.h"

namespace Finch
{
    // AST node for a named object: "foo"
    class NameExpr : public Expr
    {
    public:
        NameExpr(String name)
        :   mName(name)
        {
            std::cout << "parse: name " << mName << std::endl;
        }
        
        virtual void Trace(std::ostream & stream) const;

    private:
        String  mName;
    };
}