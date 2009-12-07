#pragma once

#include <iostream>
#include <memory>

#include "Macros.h"
#include "Expr.h"
#include "String.h"

namespace Finch
{
    using std::auto_ptr;
    
    // AST node for a binary message send: "obj + arg"
    class OperatorExpr : public Expr
    {
    public:
        OperatorExpr(auto_ptr<Expr> object, String op, auto_ptr<Expr> arg)
        :   mObject(object),
            mOperator(op),
            mArg(arg)
        {}
        
        virtual void Trace(std::ostream & stream) const;

    private:
        // the object receiving the message
        auto_ptr<Expr> mObject;
        
        // the name of the operator
        String mOperator;
        
        // the argument being passed
        auto_ptr<Expr> mArg;
    };
}