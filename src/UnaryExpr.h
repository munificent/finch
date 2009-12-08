#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "Ref.h"
#include "String.h"

namespace Finch
{
    // AST node for an unary message send: "obj message"
    class UnaryExpr : public Expr
    {
    public:
        UnaryExpr(Ref<Expr> object, String message)
        :   mObject(object),
            mMessage(message)
        {}
        
        virtual void Trace(std::ostream & stream) const;

    private:
        // the object receiving the message
        Ref<Expr> mObject;
        
        // the name of the message
        String mMessage;
    };
}