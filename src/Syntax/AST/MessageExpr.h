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
    
    // Represents a single message send to some receiver.
    class MessageSend
    {
    public:
        MessageSend()
        :   mName(),
            mArguments()
        {}
        
        MessageSend(const String & name, const Array<Ref<Expr> > & arguments)
        :   mName(name),
            mArguments(arguments)
        {}
        
        MessageSend & operator=(const MessageSend & other)
        {
            // Early out of self-assignment.
            if (&other == this) return *this;
            
            mName = other.mName;
            mArguments = other.mArguments;
            
            return *this;
        }
        
        String                    Name()      const { return mName; }
        const Array<Ref<Expr> > & Arguments() const { return mArguments; }
              Array<Ref<Expr> > & Arguments()       { return mArguments; }

    private:
        
        // The name of the message.
        String mName;
        
        // The arguments being passed.
        Array<Ref<Expr> > mArguments;
    };
    
    // AST node for a message send. Handles unary, binary, and keyword messages.
    class MessageExpr : public Expr
    {
    public:
        MessageExpr(Ref<Expr> receiver)
        :   mReceiver(receiver)
        {
        }
        
        MessageExpr(Ref<Expr> receiver, String message, const Array<Ref<Expr> > & args)
        :   mReceiver(receiver)
        {
            mMessages.Add(MessageSend(message, args));
        }
        
        Ref<Expr>                  Receiver() const { return mReceiver; }
        const Array<MessageSend> & Messages() const { return mMessages; }
              Array<MessageSend> & Messages()       { return mMessages; }

        void AddSend(String name, const Array<Ref<Expr> > & args)
        {
            mMessages.Add(MessageSend(name, args));
        }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mReceiver << " ";
            
            for (int i = 0; i < mMessages.Count(); i++)
            {
                if (i > 0) stream << ", ";
                
                const MessageSend & message = mMessages[i];
                
                stream << message.Name() << "(";
                
                if (message.Arguments().Count() > 0)
                {
                    stream << message.Arguments()[0];
                }
                
                for (int i = 1; i < message.Arguments().Count(); i++)
                {
                    stream << ", " << message.Arguments()[i];
                }
                stream << ")";
            }
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        Array<MessageSend> mMessages;
    };
}

