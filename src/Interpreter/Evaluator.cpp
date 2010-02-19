#include <iostream>

#include "Evaluator.h"

#include "Environment.h"
#include "BlockExpr.h"
#include "DefExpr.h"
#include "KeywordExpr.h"
#include "NameExpr.h"
#include "NumberExpr.h"
#include "OperatorExpr.h"
#include "SequenceExpr.h"
#include "SetExpr.h"
#include "StringExpr.h"
#include "UnaryExpr.h"

namespace Finch
{
    Ref<Object> Evaluator::Evaluate(Ref<Expr> expr)
    {
        return expr->Accept(*this);
    }
    
    Ref<Object> Evaluator::Visit(const BlockExpr & expr)
    {
        return Object::NewBlock(mEnvironment, expr.Params(), expr.Body());
    }
    
    Ref<Object> Evaluator::Visit(const DefExpr & expr)
    {
        Ref<Object> value;
        
        if (!expr.Value().IsNull())
        {
            value = expr.Value()->Accept(*this);
        }
        
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL:
                mEnvironment.Globals()->Define(expr.Name(), value);
                break;
                
            case NAMESCOPE_OBJECT:
                if (!mEnvironment.Self().IsNull())
                {
                    mEnvironment.Self()->ObjectScope()->Define(expr.Name(), value);
                }
                else
                {
                    mEnvironment.RuntimeError("Cannot define an object variable outside of a method.");
                }
                break;
                
            case NAMESCOPE_LOCAL:
                mEnvironment.CurrentScope()->Define(expr.Name(), value);
                break;
        }
        
        return NullToNil(value);
    }
    
    Ref<Object> Evaluator::Visit(const KeywordExpr & expr)
    {
        // get the object receiving the message
        Ref<Object> receiver = expr.Receiver()->Accept(*this);
        
        // evaluate the arguments
        String               fullName;
        vector<Ref<Object> > args;
        
        for (unsigned int i = 0; i < expr.Keywords().size(); i++)
        {
            fullName += expr.Keywords()[i];
            args.push_back(expr.Arguments()[i]->Accept(*this));
        }
        
        // send the message
        return NullToNil(receiver->Receive(receiver, mEnvironment, fullName, args));
    }
    
    Ref<Object> Evaluator::Visit(const NameExpr & expr)
    {
        if (expr.Name() == "self") return mEnvironment.Self();
        
        Ref<Object> result;
        
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL:
                result = mEnvironment.Globals()->LookUp(expr.Name());
                break;
                
            case NAMESCOPE_OBJECT:
                if (!mEnvironment.Self().IsNull())
                {
                    result = mEnvironment.Self()->ObjectScope()->LookUp(expr.Name());
                }
                break;
                
            case NAMESCOPE_LOCAL:
                result = mEnvironment.CurrentScope()->LookUp(expr.Name());
                break;
        }
        
        return NullToNil(result);
    }

    Ref<Object> Evaluator::Visit(const NumberExpr & expr)
    {
        return Object::NewNumber(mEnvironment, expr.Value());
    }
    
    Ref<Object> Evaluator::Visit(const OperatorExpr & expr)
    {
        Ref<Object> receiver = expr.Receiver()->Accept(*this);
        Ref<Object> arg      = expr.Argument()->Accept(*this);
        
        vector<Ref<Object> > args;
        args.push_back(arg);
        
        return NullToNil(receiver->Receive(receiver, mEnvironment, expr.Operator(), args));
    }    
    
    Ref<Object> Evaluator::Visit(const SequenceExpr & expr)
    {
        // evaluate the first, discarding the result
        Ref<Object> first = expr.First()->Accept(*this);
        
        // evaluate the second
        return expr.Second()->Accept(*this);
    }
    
    Ref<Object> Evaluator::Visit(const SetExpr & expr)
    {
        Ref<Object> value = expr.Value()->Accept(*this);
        
        Ref<Object> result;
        
        switch (Expr::GetNameScope(expr.Name()))
        {
            case NAMESCOPE_GLOBAL:
                result = mEnvironment.Globals()->Set(expr.Name(), value);
                break;
                
            case NAMESCOPE_OBJECT:
                if (!mEnvironment.Self().IsNull())
                {
                    result = mEnvironment.Self()->ObjectScope()->Set(expr.Name(), value);
                }
                else
                {
                    mEnvironment.RuntimeError("Cannot set an object variable outside of a method.");
                }
                break;
                
            case NAMESCOPE_LOCAL:
                result = mEnvironment.CurrentScope()->Set(expr.Name(), value);
                break;
        }
        
        return NullToNil(result);
    }
    
    Ref<Object> Evaluator::Visit(const StringExpr & expr)
    {
        return Object::NewString(mEnvironment, expr.Value());
    }
    
    Ref<Object> Evaluator::Visit(const UnaryExpr & expr)
    {
        Ref<Object> receiver = expr.Receiver()->Accept(*this);
        
        return NullToNil(receiver->Receive(receiver, mEnvironment, expr.Message(), vector<Ref<Object> >()));
    }
        
    Ref<Object> Evaluator::NullToNil(Ref<Object> result) const
    {
        if (result.IsNull()) return mEnvironment.Nil();
        
        return result;
    }
}