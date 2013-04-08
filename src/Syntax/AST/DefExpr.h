#pragma once

#include <iostream>

#include "Array.h"
#include "DefineExpr.h"
#include "Expr.h"
#include "IExprCompiler.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
        
    // AST node for a "def" expression.
    class DefExpr : public DefineExpr
    {
    public:
        DefExpr(String name)
        :   mName(name)
        {}
        
        String Name() const { return mName; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << "def " << mName << " {" << std::endl;

            for (int i = 0; i < Definitions().Count(); i++)
            {
                stream << Definitions()[0].GetName() << " "
                       << Definitions()[0].GetBody();
            }

            stream << "}";
        }
        
        EXPRESSION_VISITOR
        
    private:
        // The name of the object the properties are being defined on.
        String mName;
    };
}

