#pragma once

#include <iostream>

#include "Array.h"
#include "Expr.h"
#include "IExprVisitor.h"
#include "Macros.h"
#include "Ref.h"
#include "FinchString.h"

namespace Finch
{
    using std::ostream;
    
    // AST node for a message send. Handles unary, binary, and keyword messages.
    class MessageExpr : public Expr
    {
    public:
        MessageExpr(Ref<Expr> receiver, String message, const Array<Ref<Expr> > & args)
        :   mReceiver(receiver),
            mMessage(message),
            mArgs(args)
        {
        }
        
        Ref<Expr>                 Receiver()  const { return mReceiver; }
        String                    Message()   const { return mMessage; }
        const Array<Ref<Expr> > & Arguments() const { return mArgs; }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mReceiver << " " << mMessage << " [";
            
            if (mArgs.Count() > 0) stream << mArgs[0];
            for (int i = 1; i < mArgs.Count(); i++)
            {
                stream << "; " << mArgs[i];
            }
            stream << "]";
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        // the name of the message
        String mMessage;
        
        // the arguments being passed
        Array<Ref<Expr> > mArgs;
    };
}

