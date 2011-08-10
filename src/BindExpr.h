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
    
    // A single method definition.
    class Definition
    {
    public:
        Definition()
        :   mName(),
            mBody()
        {}
        
        Definition(const String & name, const Ref<Expr> & body)
        :   mName(name),
            mBody(body)
        {}
        
        Definition & operator=(const Definition & other)
        {
            // Early out of self-assignment.
            if (&other == this) return *this;
            
            mName = other.mName;
            mBody = other.mBody;
            
            return *this;
        }
        
              String      GetName() const { return mName; }
        const Ref<Expr> & GetBody() const { return mBody; }
        
    private:
        
        // The name of the message.
        String mName;
        
        // The method body. The referred-to Expr should be a Blcoc.
        Ref<Expr> mBody;
    };
    
    // AST node for a series of method definitions on some target object.
    class BindExpr : public Expr
    {
    public:
        BindExpr(Ref<Expr> target)
        :   mTarget(target)
        {
        }
        
        Ref<Expr>                 Target() const { return mTarget; }
        const Array<Definition> & Methods() const { return mMethods; }
        
        void AddMethod(String name, const Ref<Expr> & body)
        {
            mMethods.Add(Definition(name, body));
        }
        
        virtual void Trace(ostream & stream) const
        {
            stream << mTarget << " :: ";
            
            if (mMethods.Count() == 1)
            {
                stream << mMethods[0].GetName() << " " << mMethods[0].GetBody();
            }
            else
            {
                stream << "( ";
                
                for (int i = 0; i < mMethods.Count(); i++)
                {
                    stream << mMethods[0].GetName() << " "
                           << mMethods[0].GetBody();
                }
                
                stream << " )";
            }
        }
        
        EXPRESSION_VISITOR
        
    private:
        // The object the methods are being defined on.
        Ref<Expr> mTarget;
        
        Array<Definition> mMethods;
    };
}

