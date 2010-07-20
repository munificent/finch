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
        
        String                    GetName()      const { return mName; }
        const Array<Ref<Expr> > & GetArguments() const { return mArguments; }
        
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
        MessageExpr(Ref<Expr> receiver, String message, const Array<Ref<Expr> > & args)
        :   mReceiver(receiver)
        {
            mMessages.Add(MessageSend(message, args));
        }
        
        Ref<Expr>                  Receiver() const { return mReceiver; }
        const Array<MessageSend> & Messages() const { return mMessages; }
        
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
                
                stream << message.GetName() << " [";
                
                if (message.GetArguments().Count() > 0)
                {
                    stream << message.GetArguments()[0];
                }
                
                for (int i = 1; i < message.GetArguments().Count(); i++)
                {
                    stream << "; " << message.GetArguments()[i];
                }
                stream << "]";
            }
        }
        
        EXPRESSION_VISITOR
        
    private:
        // the object receiving the message
        Ref<Expr> mReceiver;
        
        Array<MessageSend> mMessages;
    };
}

