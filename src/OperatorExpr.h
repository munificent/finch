#pragma once

#include <iostream>
#include <memory>

#include "Macros.h"
#include "Expr.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    // AST node for a binary message send: "obj + arg"
    class OperatorExpr : public Expr
    {
    public:
        OperatorExpr(Ref<Expr> object, String op, Ref<Expr> arg)
        :   mObject(object),
            mOperator(op),
            mArg(arg)
        {}
        
        virtual void Trace(std::ostream & stream) const;

    private:
        // the object receiving the message
        Ref<Expr> mObject;
        
        // the name of the operator
        String mOperator;
        
        // the argument being passed
        Ref<Expr> mArg;
    };
}