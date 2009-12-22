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
        //### bob: what happens if you define a variable already defined in
        // this scope?
        Ref<Object> value;
        
        if (!expr.Value().IsNull())
        {
            value = expr.Value()->Accept(*this);
        }
        
        return mEnvironment.CurrentScope()->Define(expr.Name(), value);
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
            fullName.append(expr.Keywords()[i]);
            args.push_back(expr.Arguments()[i]->Accept(*this));
        }
        
        // send the message
        return NullToNil(receiver->Receive(receiver, mEnvironment, fullName, args));
    }
    
    Ref<Object> Evaluator::Visit(const NameExpr & expr)
    {
        if (expr.Name() == "self") return mEnvironment.Self();
        
        //### bob: hack temp. always look up in global scope
        return NullToNil(mEnvironment.CurrentScope()->LookUp(expr.Name()));
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
        
        return mEnvironment.CurrentScope()->Set(expr.Name(), value);
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