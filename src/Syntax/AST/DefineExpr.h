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
    
    // A single method or object variable definition in a bind expression or an
    // object literal.
    class Definition
    {
    public:
        // So we can use definitions in an Array<T>.
        Definition()
        :   mIsMethod(false),
            mName(),
            mBody()
        {}
        
        Definition(bool isMethod, const String & name, const Ref<Expr> & body)
        :   mIsMethod(isMethod),
            mName(name),
            mBody(body)
        {}
        
        Definition & operator=(const Definition & other)
        {
            // Early out of self-assignment.
            if (&other == this) return *this;
            
            mIsMethod = other.mIsMethod;
            mName = other.mName;
            mBody = other.mBody;
            
            return *this;
        }
        
              bool        IsMethod() const { return mIsMethod; }
              String      GetName() const { return mName; }
        const Ref<Expr> & GetBody() const { return mBody; }
        
    private:
        // True if this definition is a method, false for a variable.
        bool mIsMethod;
        
        // The name of the message.
        String mName;
        
        // The method body. The referred-to Expr should be a BlockExpr.
        Ref<Expr> mBody;
    };
    
    // Base class for an AST node that has a collection of Definitions.
    // Inherited by BindExpr (for adding to an existing object) and ObjectExpr
    // (for creating a new one).
    class DefineExpr : public Expr
    {
    public:
        DefineExpr()
        {}
        
        const Array<Definition> & Definitions() const { return mDefinitions; }
        
        void Define(bool isMethod, String name, const Ref<Expr> & body)
        {
            mDefinitions.Add(Definition(isMethod, name, body));
        }
        
    private:
        Array<Definition> mDefinitions;
    };
}

