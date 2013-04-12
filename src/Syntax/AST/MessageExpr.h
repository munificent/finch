#pragma once

#include <iostream>

#include "Array.h"
#include "Expr.h"
#include "IExprCompiler.h"
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
        MessageExpr(Ref<Expr> receiver, String name,
                    const Array<Ref<Expr> > & args)
        :   mReceiver(receiver),
            mName(name),
            mArguments(args)
        {}
        
        Ref<Expr>                 Receiver()  const { return mReceiver; }
        String                    Name()      const { return mName; }
        const Array<Ref<Expr> > & Arguments() const { return mArguments; }
              Array<Ref<Expr> > & Arguments()       { return mArguments; }

        virtual void Trace(ostream & stream) const
        {
            stream << mReceiver << "." << mName;

            if (mArguments.Count() > 0)
            {
                stream << "(" << mArguments[0];
                for (int i = 1; i < mArguments.Count(); i++)
                {
                    stream << ", " << mArguments[i];
                }
                stream << ")";
            }
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        // The name of the message.
        String mName;

        // The arguments being passed.
        Array<Ref<Expr> > mArguments;
    };
}

