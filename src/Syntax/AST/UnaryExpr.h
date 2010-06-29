#pragma once

#include <iostream>

#include "Macros.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for an unary message send: "obj message"
    class UnaryExpr : public Expr
    {
    public:
        UnaryExpr(Ref<Expr> receiver, String message)
        :   mReceiver(receiver),
            mMessage(message)
        {}
        
        Ref<Expr> Receiver() const { return mReceiver; }
        String    Message() const { return mMessage; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mReceiver << " " << mMessage;
        }
            
        EXPRESSION_VISITOR

    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        // the name of the message
        String mMessage;
    };
}

